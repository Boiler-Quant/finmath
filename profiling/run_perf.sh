#!/usr/bin/env bash
# Run perf (stat or record) on a finmath target.
# Usage:
#   ./run_perf.sh stat <target>     - run perf stat (counts, no sample)
#   ./run_perf.sh record <target>   - run perf record, then report
#   ./run_perf.sh stat <target> -- python -c "import finmath; ..."  - run perf stat on Python
#
# Requires: build with -DFINMATH_PROFILING=ON and perf available.
# Set BUILD_DIR to your CMake build dir (default: ./cmake-build-debug or ./build).

set -e
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-}"
for d in "$REPO_ROOT/build" "$REPO_ROOT/cmake-build-debug" "$REPO_ROOT/cmake-build-release"; do
  if [[ -d "$d" ]]; then
    BUILD_DIR="${BUILD_DIR:-$d}"
    break
  fi
done
if [[ -z "$BUILD_DIR" ]]; then
  echo "No build dir found. Set BUILD_DIR or build with cmake first."
  exit 1
fi

MODE="${1:-stat}"
TARGET="${2:-}"
shift 2 2>/dev/null || true
EXTRA_ARGS=("$@")

if [[ -z "$TARGET" ]]; then
  echo "Usage: $0 <stat|record> <target> [perf options...]"
  echo "  target: one of black_scholes, binomial_option_pricing, compound_interest, rolling_volatility,"
  echo "          rolling_volatility_simd, simple_moving_average, simple_moving_average_simd, rsi, rsi_simd,"
  echo "          ema_simd, simd_helper, bellman_arbitrage"
  echo "  Or: path to an executable, or 'python' plus command for Python-based profiling."
  exit 1
fi

# Resolve executable or run_via_python from profile_targets.json
EXECUTABLE=""
RUN_VIA_PYTHON=""
if [[ -f "$SCRIPT_DIR/profile_targets.json" ]]; then
  read -r EXECUTABLE RUN_VIA_PYTHON <<< $(python3 -c "
import json
with open('$SCRIPT_DIR/profile_targets.json') as f:
    d = json.load(f)
t = d.get('targets', {}).get('$TARGET', {})
print(t.get('executable', ''), t.get('run_via_python', ''))
" 2>/dev/null)
fi

if [[ -n "$RUN_VIA_PYTHON" ]]; then
  PY_SCRIPT="${RUN_VIA_PYTHON%% *}"
  PY_ARGS="${RUN_VIA_PYTHON#* }"
  if [[ "$PY_ARGS" == "$RUN_VIA_PYTHON" ]]; then
    CMD=(python3 "$REPO_ROOT/$PY_SCRIPT")
  else
    CMD=(python3 "$REPO_ROOT/$PY_SCRIPT" $PY_ARGS)
  fi
elif [[ -n "$EXECUTABLE" ]]; then
  EXE_PATH="$BUILD_DIR/$EXECUTABLE"
  if [[ ! -x "$EXE_PATH" ]]; then
    echo "Executable not found: $EXE_PATH (build with -DFINMATH_PROFILING=ON)"
    exit 1
  fi
  CMD=("$EXE_PATH")
else
  # Assume TARGET is a path to executable or "python"
  if [[ "$TARGET" == "python" ]]; then
    PYTHON_CMD=("${EXTRA_ARGS[@]}")
    EXTRA_ARGS=()
    if [[ ${#PYTHON_CMD[@]} -eq 0 ]]; then
      echo "For Python profiling use: $0 $MODE python python script.py"
      exit 1
    fi
    CMD=("${PYTHON_CMD[@]}")
  else
    if [[ -x "$TARGET" ]]; then
      CMD=("$TARGET")
    elif [[ -x "$BUILD_DIR/$TARGET" ]]; then
      CMD=("$BUILD_DIR/$TARGET")
    else
      echo "Unknown target and not an executable: $TARGET"
      exit 1
    fi
  fi
fi

if ! command -v perf &>/dev/null; then
  echo "perf is not available. It is typically installed on Linux (linux-tools or perf-tools)."
  echo "On macOS you can use Instruments, or use: ./profile llvm-mca <target> for machine-code analysis."
  exit 1
fi

case "$MODE" in
  stat)
    exec perf stat "${EXTRA_ARGS[@]}" -- "${CMD[@]}"
    ;;
  record)
    PERF_DATA="${PERF_DATA:-$REPO_ROOT/perf.data}"
    perf record -o "$PERF_DATA" -g "${EXTRA_ARGS[@]}" -- "${CMD[@]}"
    echo "Saved to $PERF_DATA. Run: perf report -i $PERF_DATA"
    perf report -i "$PERF_DATA"
    ;;
  *)
    echo "Mode must be 'stat' or 'record'"
    exit 1
    ;;
esac
