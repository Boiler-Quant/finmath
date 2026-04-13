"""
Pytest configuration and shared fixtures for finmath Python tests.

Ensures the built extension is importable: either install the package
(pip install -e .) or set PYTHONPATH to include the project root and src/.
"""
import sys
from pathlib import Path

# Project root on path so package `finmath/` (with built `_finmath` extension) is importable
_root = Path(__file__).resolve().parent.parent
if _root not in sys.path:
    sys.path.insert(0, str(_root))
