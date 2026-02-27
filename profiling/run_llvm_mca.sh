#!/usr/bin/env bash
# Generate assembly for a finmath source and run llvm-mca (machine code analyzer).
# Usage:
#   ./run_llvm_mca.sh <target> [--mcpu=CPU] [--iterations=N]
#
# Target: short name (e.g. black_scholes) or path to .cpp file.
# Requires: Clang and llvm-mca on PATH (e.g. from LLVM).
# Optional: --mcpu=haswell (or native), --iterations=10

set -e
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-$REPO_ROOT/cmake-build-debug}"
MCA_CPU="${MCA_CPU:-native}"
MCA_ITERATIONS="${MCA_ITERATIONS:-10}"

TARGET="${1:-}"
shift || true
while [[ $# -gt 0 ]]; do
  case "$1" in
    --mcpu=*) MCA_CPU="${1#--mcpu=}"; shift ;;
    --iterations=*) MCA_ITERATIONS="${1#--iterations=}"; shift ;;
    *) break ;;
  esac
done

if [[ -z "$TARGET" ]]; then
  echo "Usage: $0 <target> [--mcpu=CPU] [--iterations=N]"
  echo "  target: black_scholes, binomial_option_pricing, compound_interest, rolling_volatility,"
  echo "          rolling_volatility_simd, simple_moving_average, simple_moving_average_simd, rsi, rsi_simd,"
  echo "          ema_simd, simd_helper, bellman_arbitrage, or path to .cpp"
  exit 1
fi

# Resolve source file from profile_targets.json
SRC_FILE=""
if [[ -f "$SCRIPT_DIR/profile_targets.json" ]]; then
  SRC_FILE=$(python3 -c "
import json, sys
with open('$SCRIPT_DIR/profile_targets.json') as f:
    d = json.load(f)
t = d.get('targets', {}).get('$TARGET', {})
print(t.get('source_for_mca', ''))
" 2>/dev/null)
  if [[ -n "$SRC_FILE" ]]; then
    SRC_FILE="$REPO_ROOT/$SRC_FILE"
  fi
fi

if [[ -z "$SRC_FILE" ]]; then
  if [[ -f "$TARGET" && "${TARGET##*.}" == "cpp" ]]; then
    SRC_FILE="$TARGET"
  elif [[ -f "$REPO_ROOT/$TARGET" ]]; then
    SRC_FILE="$REPO_ROOT/$TARGET"
  else
    echo "Unknown target and not a .cpp path: $TARGET"
    exit 1
  fi
fi

if [[ ! -f "$SRC_FILE" ]]; then
  echo "Source not found: $SRC_FILE"
  exit 1
fi

# Prefer clang++; fallback to clang
CXX="${CXX:-clang++}"
if ! command -v "$CXX" &>/dev/null; then
  CXX="clang++"
fi
if ! command -v llvm-mca &>/dev/null; then
  echo "llvm-mca not found. Install LLVM (e.g. brew install llvm) and ensure llvm-mca is on PATH."
  exit 1
fi

# Include and flags similar to project (optimized, no link)
INCLUDES=(-I"$REPO_ROOT/include")
# Optional: match project SIMD flags
if [[ "$(uname -m)" == "x86_64" ]]; then
  INCLUDES+=(-msse2 -mavx)
fi

echo "Source: $SRC_FILE"
echo "CXX: $CXX  mcpu=$MCA_CPU  iterations=$MCA_ITERATIONS"
echo "--- Assembly + llvm-mca ---"

# Emit assembly and pipe to llvm-mca (no linking)
"$CXX" -std=c++17 -O3 -S -o - "${INCLUDES[@]}" "$SRC_FILE" \
  | llvm-mca -mcpu="$MCA_CPU" -iterations="$MCA_ITERATIONS" "$@"
