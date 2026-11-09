@echo off
rem CAN hardware tests with the CANivore Py Test Tool (submodule tools\canivore_py_test_tool).
rem Creates an isolated Python environment under .pio\ on first use, installs the submodule
rem in editable mode and passes all arguments to canivore-py-test-tool.
rem   .\scripts\can_test.cmd --help
rem   .\scripts\can_test.cmd tx-pattern --port 22 --can-bitrate 500000 --tx-id 611 --stream-id 20 --tx-count 100 --tx-interval-ms 10
setlocal
set "ROOT=%~dp0.."
set "VENV=%ROOT%\.pio\canivore-py-test-tool-venv"
set "TOOL=%ROOT%\tools\canivore_py_test_tool"

if not exist "%TOOL%\pyproject.toml" (
    echo Submodule missing: run  git submodule update --init --recursive
    exit /b 1
)
if not exist "%VENV%\Scripts\python.exe" (
    echo Creating Python environment in %VENV% ...
    python -m venv "%VENV%" || exit /b 1
    "%VENV%\Scripts\python.exe" -m pip install --quiet --upgrade pip || exit /b 1
    "%VENV%\Scripts\python.exe" -m pip install --quiet -e "%TOOL%" pyserial || exit /b 1
)
"%VENV%\Scripts\canivore-py-test-tool.exe" %*
exit /b %ERRORLEVEL%
