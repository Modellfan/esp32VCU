@AGENTS.md

## Hinweise für Claude Code
- `$re-refine` und `$re-review` aus `AGENTS.md` entsprechen den Skills `/re-refine` und `/re-review` (`.claude/skills/`). `.agents/skills/` enthält dieselben Skills für Codex.
- PlatformIO liegt unter `%USERPROFILE%\.platformio\penv\Scripts\pio.exe` (nicht im PATH). Build: `pio run -e esp32s3`.
- Compiler-Problem der pioarduino-Plattform unter Windows und Abhilfe: README, Abschnitt „Build und Flashen“.
- Logiktests auf dem PC (`pio test -e native`) gehen hier nicht: kein Host-Compiler installiert.
