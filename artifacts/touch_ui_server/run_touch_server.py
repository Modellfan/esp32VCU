import pathlib
import sys

repo = pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0, str(repo / "tools"))
from touch_ui_test_support import start_server

server, thread = start_server("127.0.0.1", 8080)
print("touch-ui server listening on http://127.0.0.1:8080/touch.html", flush=True)
thread.join()
