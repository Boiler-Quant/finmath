# Finmath profiling and performance setup

This directory provides a single way to run **instrumentation**, **statistical profiling**, and **machine-code analysis** on any function (or module) in finmath.

**Platform note:** `perf` and `gprof` are commonly available on Linux. On macOS, `perf` is not installed by default; use **llvm-mca** (e.g. `brew install llvm`) for machine-code analysis, or Xcode Instruments for sampling. The scripts check for required tools and print clear messages if they are missing.

## C++ vs Python: what to profile?

| Goal | Use |
|------|-----|
| **Hot C++ code** (SIMD, numerics, option pricing) | **C++ targets** – run `perf`, `gprof`, or `llvm-mca` on the C++ test executables. This is where most CPU time is spent. |
| **Full stack** (Python + C++ together) | **Python** – run `perf record` (or `py-spy`) on a Python process that calls finmath (e.g. `python benchmark/benchmark.py`). You still see C++ symbols if the build has debug symbols. |

**Recommendation:** Use **C++ targets** for optimization work (perf, llvm-mca, gprof). Use **Python** only when you care about call patterns or overhead from the Python side.

---

## 1. Build for profiling

Profiling needs symbols and (for good call stacks) frame pointers. From the repo root (scripts look for `build/`, `cmake-build-debug/`, or `cmake-build-release/`; set `BUILD_DIR` if you use another directory):

```bash
mkdir -p build && cd build
cmake -DFINMATH_PROFILING=ON ..
# Optional, for gprof only:
# cmake -DFINMATH_PROFILING=ON -DFINMATH_INSTRUMENTATION=ON ..
make -j
cd ..
```

- **`FINMATH_PROFILING=ON`**: adds `-g -fno-omit-frame-pointer` (for `perf`, sampling, and readable stacks).
- **`FINMATH_INSTRUMENTATION=ON`**: adds `-pg` for **gprof** (instrumentation). Use with `run_gprof.sh` / `profile gprof`.

---

## 2. Entry point: `./profiling/profile`

From the repo root:

```bash
./profiling/profile list                    # list available targets
./profiling/profile perf-stat black_scholes
./profiling/profile perf-record black_scholes
./profiling/profile gprof black_scholes
./profiling/profile llvm-mca black_scholes
```

Targets are defined in `profiling/profile_targets.json` and map to C++ test executables and source files.

---

## 3. Statistical profiling: `perf`

**perf** does low-overhead sampling (no recompilation). Use it to see where time is spent.

- **Counts (cycles, instructions, cache, etc.):**
  ```bash
  ./profiling/run_perf.sh stat black_scholes
  # or
  ./profiling/profile perf-stat black_scholes
  ```
- **Sample and get a call-graph report:**
  ```bash
  ./profiling/run_perf.sh record black_scholes
  # writes perf.data, then runs perf report
  ./profiling/profile perf-record black_scholes
  ```

**Python (full stack):**

```bash
./profiling/run_perf.sh stat python -- python -c "
import finmath
import numpy as np
x = np.random.randn(100000).astype(np.float64)
for _ in range(50):
    finmath.simple_moving_average_simd(x, 14)
"
```

Requires: `perf` installed (Linux: `perf`, macOS: some features via `dtrace` or Instruments; `perf` is Linux-focused).

---

## 4. Instrumentation: gprof

**gprof** uses compiler instrumentation (`-pg`) to count calls and time per function.

1. Build with instrumentation:
   ```bash
   cd build && cmake -DFINMATH_PROFILING=ON -DFINMATH_INSTRUMENTATION=ON .. && make -j && cd ..
   ```
2. Run and get report:
   ```bash
   ./profiling/run_gprof.sh black_scholes
   # or
   ./profiling/profile gprof black_scholes
   ```
   Report is written to `profiling/gprof_<target>.txt`.

---

## 5. Machine-code analysis: llvm-mca

**llvm-mca** simulates CPU pipeline and estimates throughput/bottlenecks from assembly (no execution).

```bash
./profiling/run_llvm_mca.sh black_scholes
# or with options
./profiling/run_llvm_mca.sh black_scholes --mcpu=haswell --iterations=20
./profiling/profile llvm-mca black_scholes --iterations=20
```

You can pass any target name from `profile_targets.json` or a path to a `.cpp` file. Requires **Clang** and **llvm-mca** (e.g. `brew install llvm` on macOS, then ensure `llvm-mca` is on `PATH`).

---

## 6. Adding a new target

Edit `profiling/profile_targets.json`:

1. Under **`targets`**, add an entry, e.g.:
   - `"my_func"`: `executable` = C++ test binary that exercises it (e.g. `MyFuncTest_executable`),
   - `source_for_mca`: path to the `.cpp` that implements it (e.g. `src/cpp/Module/my_func.cpp`),
   - `symbol`: optional function name for reference.
2. If you have a **Python** driver script that stresses a specific API, add it under **`python_drivers`** and run `perf` with that script as the command.

---

## 7. Tools summary

| Tool | Type | Use case |
|------|------|----------|
| **perf stat** | Statistical | Instruction counts, cycles, cache misses. |
| **perf record / report** | Statistical | Where time is spent, call graph. |
| **gprof** | Instrumentation | Call counts and time per function (build with `-pg`). |
| **llvm-mca** | Machine-code | Throughput and bottlenecks from generated assembly. |

All of these can be run on any finmath target via the scripts in this directory and the `profile` entry point.
