"""PlatformIO pre-script: touch src/services/serial_console.cpp before every build.

The file prints __DATE__/__TIME__ (serial command "version"). Without the touch the build
date would only change when that file itself is edited. Only this one file is recompiled.
"""
import os

Import("env")  # noqa: F821 (provided by PlatformIO)

path = os.path.join(env.subst("$PROJECT_DIR"), "src", "services", "serial_console.cpp")  # noqa: F821
if os.path.exists(path):
    os.utime(path, None)
