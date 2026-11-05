from __future__ import annotations

import sys
from pathlib import Path

SIL = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(SIL))

# Each test module defines a fixture for its component, e.g. VcuSil("cluster").
