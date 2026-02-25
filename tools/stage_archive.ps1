param(
  [Parameter(Mandatory = $true)]
  [ValidateSet("snapshot", "list", "restore")]
  [string]$Action,

  [string]$Label = "",
  [string]$Stage = "",
  [string[]]$Paths = @(),
  [switch]$DryRun
)

$ErrorActionPreference = "Stop"

function Get-RepoRoot {
  $root = (git rev-parse --show-toplevel 2>$null)
  if (-not $root) {
    throw "Not inside a git repository."
  }
  return $root.Trim()
}

function Ensure-Dir([string]$Path) {
  if (-not (Test-Path -LiteralPath $Path)) {
    New-Item -ItemType Directory -Path $Path | Out-Null
  }
}

function Get-RelativePathCompat([string]$BasePath, [string]$TargetPath) {
  $base = [System.IO.Path]::GetFullPath($BasePath)
  $target = [System.IO.Path]::GetFullPath($TargetPath)
  if (-not $base.EndsWith([System.IO.Path]::DirectorySeparatorChar)) {
    $base = $base + [System.IO.Path]::DirectorySeparatorChar
  }
  $baseUri = New-Object System.Uri($base)
  $targetUri = New-Object System.Uri($target)
  $relUri = $baseUri.MakeRelativeUri($targetUri)
  $rel = [System.Uri]::UnescapeDataString($relUri.ToString())
  return ($rel -replace '/', '\')
}

function Safe-Label([string]$Text) {
  if ([string]::IsNullOrWhiteSpace($Text)) {
    return "stage"
  }
  $safe = $Text -replace "[^A-Za-z0-9._-]", "_"
  if ([string]::IsNullOrWhiteSpace($safe)) {
    return "stage"
  }
  return $safe
}

function Resolve-ArchiveRoot([string]$RepoRoot) {
  return Join-Path $RepoRoot "archives\stages"
}

function Resolve-StagePath([string]$ArchiveRoot, [string]$StageId) {
  if ([string]::IsNullOrWhiteSpace($StageId)) {
    throw "Stage id is required."
  }
  return Join-Path $ArchiveRoot $StageId
}

function Build-Manifest([string]$RepoRoot, [string]$StageId, [string]$Label, [string[]]$ResolvedFiles) {
  $localNow = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
  $utcNow = (Get-Date).ToUniversalTime().ToString("yyyy-MM-dd HH:mm:ssZ")
  $head = (git -C $RepoRoot rev-parse --short HEAD 2>$null).Trim()
  $status = (git -C $RepoRoot status --short 2>$null)

  return [ordered]@{
    stage_id         = $StageId
    label            = $Label
    local_time       = $localNow
    utc_time         = $utcNow
    git_head         = $head
    archived_files   = $ResolvedFiles
    git_status_short = $status
  }
}

function Snapshot-Stage {
  $repoRoot = Get-RepoRoot
  $archiveRoot = Resolve-ArchiveRoot $repoRoot
  Ensure-Dir $archiveRoot

  if ($Paths.Count -eq 0) {
    throw "For snapshot, provide -Paths with one or more repo-relative files or directories."
  }

  $safe = Safe-Label $Label
  $ts = Get-Date -Format "yyyyMMdd_HHmmss"
  $stageId = "${ts}_${safe}"
  $stagePath = Join-Path $archiveRoot $stageId
  Ensure-Dir $stagePath

  $copied = New-Object System.Collections.Generic.List[string]
  foreach ($p in $Paths) {
    $full = Join-Path $repoRoot $p
    if (-not (Test-Path -LiteralPath $full)) {
      throw "Path not found: $p"
    }
    if ((Get-Item -LiteralPath $full).PSIsContainer) {
      Get-ChildItem -LiteralPath $full -Recurse -File | ForEach-Object {
        $src = $_.FullName
        $rel = Get-RelativePathCompat -BasePath $repoRoot -TargetPath $src
        $dst = Join-Path $stagePath $rel
        Ensure-Dir ([System.IO.Path]::GetDirectoryName($dst))
        Copy-Item -LiteralPath $src -Destination $dst -Force
        $copied.Add($rel) | Out-Null
      }
    } else {
      $rel = Get-RelativePathCompat -BasePath $repoRoot -TargetPath $full
      $dst = Join-Path $stagePath $rel
      Ensure-Dir ([System.IO.Path]::GetDirectoryName($dst))
      Copy-Item -LiteralPath $full -Destination $dst -Force
      $copied.Add($rel) | Out-Null
    }
  }

  $copiedSorted = $copied | Sort-Object -Unique
  $manifest = Build-Manifest -RepoRoot $repoRoot -StageId $stageId -Label $Label -ResolvedFiles $copiedSorted
  $manifestPath = Join-Path $stagePath "manifest.json"
  ($manifest | ConvertTo-Json -Depth 5) | Set-Content -LiteralPath $manifestPath -Encoding UTF8

  Write-Host "SNAPSHOT_OK stage=$stageId files=$($copiedSorted.Count)"
  Write-Host "stage_path=$stagePath"
}

function List-Stages {
  $repoRoot = Get-RepoRoot
  $archiveRoot = Resolve-ArchiveRoot $repoRoot
  if (-not (Test-Path -LiteralPath $archiveRoot)) {
    Write-Host "No stages found: $archiveRoot"
    return
  }

  $dirs = Get-ChildItem -LiteralPath $archiveRoot -Directory | Sort-Object Name -Descending
  if ($dirs.Count -eq 0) {
    Write-Host "No stages found: $archiveRoot"
    return
  }

  foreach ($d in $dirs) {
    $manifestPath = Join-Path $d.FullName "manifest.json"
    if (Test-Path -LiteralPath $manifestPath) {
      $m = Get-Content -LiteralPath $manifestPath | ConvertFrom-Json
      Write-Host ("{0}  label={1}  utc={2}  files={3}" -f $d.Name, $m.label, $m.utc_time, $m.archived_files.Count)
    } else {
      Write-Host ("{0}  (no manifest)" -f $d.Name)
    }
  }
}

function Restore-Stage {
  $repoRoot = Get-RepoRoot
  $archiveRoot = Resolve-ArchiveRoot $repoRoot
  $stagePath = Resolve-StagePath -ArchiveRoot $archiveRoot -StageId $Stage
  if (-not (Test-Path -LiteralPath $stagePath)) {
    throw "Stage not found: $Stage"
  }
  $manifestPath = Join-Path $stagePath "manifest.json"
  if (-not (Test-Path -LiteralPath $manifestPath)) {
    throw "manifest.json missing in stage: $Stage"
  }

  $manifest = Get-Content -LiteralPath $manifestPath | ConvertFrom-Json
  foreach ($rel in $manifest.archived_files) {
    if ($rel -eq "manifest.json") {
      continue
    }
    $src = Join-Path $stagePath $rel
    $dst = Join-Path $repoRoot $rel
    if (-not (Test-Path -LiteralPath $src)) {
      continue
    }
    if ($DryRun) {
      Write-Host "DRYRUN restore $rel"
      continue
    }
    Ensure-Dir ([System.IO.Path]::GetDirectoryName($dst))
    Copy-Item -LiteralPath $src -Destination $dst -Force
    Write-Host "RESTORED $rel"
  }

  if ($DryRun) {
    Write-Host "RESTORE_DRYRUN_OK stage=$Stage"
  } else {
    Write-Host "RESTORE_OK stage=$Stage"
  }
}

switch ($Action) {
  "snapshot" { Snapshot-Stage; break }
  "list" { List-Stages; break }
  "restore" { Restore-Stage; break }
}
