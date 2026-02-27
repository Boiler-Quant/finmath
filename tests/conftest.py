"""
Pytest configuration and shared fixtures for finmath Python tests.

Ensures the built extension is importable: either install the package
(pip install -e .) or set PYTHONPATH to include the project root and src/.
"""
import sys
from pathlib import Path

# Add project root and src so "import finmath" finds the built extension
_root = Path(__file__).resolve().parent.parent
_src = _root / "src"
if _root not in sys.path:
    sys.path.insert(0, str(_root))
if _src not in sys.path:
    sys.path.insert(0, str(_src))
