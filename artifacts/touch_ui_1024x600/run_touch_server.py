import time
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
from tools.touch_ui_test_support import start_server


server, thread = start_server("127.0.0.1", 58983)
print("http://127.0.0.1:58983/touch.html", flush=True)

while True:
    time.sleep(3600)
