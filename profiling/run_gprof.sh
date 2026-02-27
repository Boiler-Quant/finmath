#!/usr/bin/env bash
# Build with gprof instrumentation, run a target, and show gprof report.
# Usage:
#   ./run_gprof.sh <target>
#
# Requires: build with -DFINMATH_INSTRUMENTATION=ON (and -DFINMATH_PROFILING=ON recommended).
# After running, gmon.out is in the current dir; gprof produces analysis.txt.

set -e
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-}"
for d in "$REPO_ROOT/build" "$REPO_ROOT/cmake-build-debug"; do
  if [[ -d "$d" ]]; then
    BUILD_DIR="${BUILD_DIR:-$d}"
    break
  fi
done

TARGET="${1:-}"
if [[ -z "$TARGET" ]]; then
  echo "Usage: $0 <target>"
  echo "  target: black_scholes, binomial_option_pricing, compound_interest, rolling_volatility,"
  echo "          simple_moving_average_simd, rsi_simd, simd_helper, bellman_arbitrage, etc."
  echo ""
  echo "First build with instrumentation:"
  echo "  mkdir -p build && cd build"
  echo "  cmake -DFINMATH_PROFILING=ON -DFINMATH_INSTRUMENTATION=ON .."
  echo "  make -j"
  echo "  cd .. && ./profiling/run_gprof.sh black_scholes"
  exit 1
fi

# Resolve executable
EXECUTABLE=""
if [[ -f "$SCRIPT_DIR/profile_targets.json" ]]; then
  EXECUTABLE=$(python3 -c "
import json
with open('$SCRIPT_DIR/profile_targets.json') as f:
    d = json.load(f)
t = d.get('targets', {}).get('$TARGET', {})
print(t.get('executable', ''))
" 2>/dev/null)
fi

if [[ -z "$EXECUTABLE" ]]; then
  EXE_PATH="$BUILD_DIR/$TARGET"
else
  EXE_PATH="$BUILD_DIR/$EXECUTABLE"
fi

if [[ ! -x "$EXE_PATH" ]]; then
  echo "Executable not found: $EXE_PATH"
  echo "Build with: cmake -DFINMATH_PROFILING=ON -DFINMATH_INSTRUMENTATION=ON .."
  exit 1
fi

if ! command -v gprof &>/dev/null; then
  echo "gprof is not available. It is commonly available on Linux (GNU binutils)."
  echo "On macOS it is often not installed; use perf or llvm-mca instead."
  exit 1
fi

# Run from build dir so gmon.out is next to binary (gprof expects that)
cd "$(dirname "$EXE_PATH")"
./"$(basename "$EXE_PATH")"
cd "$REPO_ROOT"

GMON="$BUILD_DIR/gmon.out"
if [[ ! -f "$GMON" ]]; then
  echo "gmon.out not found. Ensure you built with -DFINMATH_INSTRUMENTATION=ON (-pg)."
  exit 1
fi

OUTPUT="$REPO_ROOT/profiling/gprof_${TARGET}.txt"
gprof "$EXE_PATH" "$GMON" > "$OUTPUT"
echo "Report written to $OUTPUT"
head -80 "$OUTPUT"
