"""finmath: C++ financial math primitives with pybind11 bindings."""

try:
    from ._finmath import *  # type: ignore[attr-defined, import-not-found]
except ImportError as e:  # pragma: no cover
    raise ImportError(
        "The finmath extension module is not built. Build with CMake "
        "(mkdir build && cd build && cmake .. && cmake --build .) or pip install ."
    ) from e

__version__ = "0.2.0"
