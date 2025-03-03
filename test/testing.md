# Adding New Tests to finmath

This document provides instructions on how to add new tests for modules and functions.

## Directory Structure

Tests are organized within the `test/` directory:

- `test/compound_interest/` — Contains tests for compound interest calculations.
- `test/black_scholes/` — Contains tests for option pricing (Black-Scholes) functions.
- `test/rsi/` — Contains tests for RSI calculations.
- `test/` — May contain general tests (like `test_finmath.cpp`).

Create a new subdirectory in `test/` if your new tests target a different module.

## Writing the Test Code

1. **Create a New Test File:**

   - Add your new test source file (e.g., `my_new_test.cpp`) inside the appropriate subdirectory, e.g., `test/my_module/`.
   - Include the necessary headers:
     ```cpp
     #include <cassert>
     #include <cmath>
     #include <iostream>
     #include "finmath/your_module_header.h"
     ```

2. **Implement Your Test Cases:**

   - Write functions to test your module’s functionality.
   - Use assertions to verify the correctness. For example:

     ```cpp
     bool almost_equal(double a, double b, double tolerance) {
         return std::abs(a - b) <= tolerance * std::max(std::abs(a), std::abs(b));
     }

     int my_module_tests() {
         double expected = /* expected value */;
         double result = your_function(/* parameters */);
         double tolerance = 0.001;
         assert(almost_equal(result, expected, tolerance));
         std::cout << "my_module test passed!" << std::endl;
         return 0;
     }

     int main() {
         my_module_tests();
         return 0;
     }
     ```

## Updating [CMakeLists.txt](http://_vscodecontentref_/0)

1. **Add an Executable:**

   - Open the root [CMakeLists.txt](http://_vscodecontentref_/1) file.
   - Add an executable target for your test:
     ```cmake
     add_executable(my_module_test test/my_module/my_new_test.cpp)
     target_link_libraries(my_module_test finmath_library)
     add_test(NAME MyModuleTest COMMAND my_module_test)
     ```

2. **Reconfigure and Build:**
   - In your build directory, run:
     ```bash
     cmake ..
     make
     ```
   - Run the tests with:
     ```bash
     ctest --output-on-failure
     ```

## Running the Tests

- For a quick run of all tests:
  ```bash
  ctest --output-on-failure
  ```
