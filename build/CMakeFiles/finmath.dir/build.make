# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.30.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.30.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/shashank/finmath

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/shashank/finmath/build

# Include any dependencies generated for this target.
include CMakeFiles/finmath.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/finmath.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/finmath.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/finmath.dir/flags.make

CMakeFiles/finmath.dir/src/cpp/Helper/helper.cpp.o: CMakeFiles/finmath.dir/flags.make
CMakeFiles/finmath.dir/src/cpp/Helper/helper.cpp.o: /Users/shashank/finmath/src/cpp/Helper/helper.cpp
CMakeFiles/finmath.dir/src/cpp/Helper/helper.cpp.o: CMakeFiles/finmath.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/shashank/finmath/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/finmath.dir/src/cpp/Helper/helper.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/finmath.dir/src/cpp/Helper/helper.cpp.o -MF CMakeFiles/finmath.dir/src/cpp/Helper/helper.cpp.o.d -o CMakeFiles/finmath.dir/src/cpp/Helper/helper.cpp.o -c /Users/shashank/finmath/src/cpp/Helper/helper.cpp

CMakeFiles/finmath.dir/src/cpp/Helper/helper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/finmath.dir/src/cpp/Helper/helper.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/shashank/finmath/src/cpp/Helper/helper.cpp > CMakeFiles/finmath.dir/src/cpp/Helper/helper.cpp.i

CMakeFiles/finmath.dir/src/cpp/Helper/helper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/finmath.dir/src/cpp/Helper/helper.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/shashank/finmath/src/cpp/Helper/helper.cpp -o CMakeFiles/finmath.dir/src/cpp/Helper/helper.cpp.s

CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/compound_interest.cpp.o: CMakeFiles/finmath.dir/flags.make
CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/compound_interest.cpp.o: /Users/shashank/finmath/src/cpp/InterestAndAnnuities/compound_interest.cpp
CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/compound_interest.cpp.o: CMakeFiles/finmath.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/shashank/finmath/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/compound_interest.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/compound_interest.cpp.o -MF CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/compound_interest.cpp.o.d -o CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/compound_interest.cpp.o -c /Users/shashank/finmath/src/cpp/InterestAndAnnuities/compound_interest.cpp

CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/compound_interest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/compound_interest.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/shashank/finmath/src/cpp/InterestAndAnnuities/compound_interest.cpp > CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/compound_interest.cpp.i

CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/compound_interest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/compound_interest.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/shashank/finmath/src/cpp/InterestAndAnnuities/compound_interest.cpp -o CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/compound_interest.cpp.s

CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/simple_interest.cpp.o: CMakeFiles/finmath.dir/flags.make
CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/simple_interest.cpp.o: /Users/shashank/finmath/src/cpp/InterestAndAnnuities/simple_interest.cpp
CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/simple_interest.cpp.o: CMakeFiles/finmath.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/shashank/finmath/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/simple_interest.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/simple_interest.cpp.o -MF CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/simple_interest.cpp.o.d -o CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/simple_interest.cpp.o -c /Users/shashank/finmath/src/cpp/InterestAndAnnuities/simple_interest.cpp

CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/simple_interest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/simple_interest.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/shashank/finmath/src/cpp/InterestAndAnnuities/simple_interest.cpp > CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/simple_interest.cpp.i

CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/simple_interest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/simple_interest.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/shashank/finmath/src/cpp/InterestAndAnnuities/simple_interest.cpp -o CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/simple_interest.cpp.s

CMakeFiles/finmath.dir/src/cpp/OptionPricing/binomial_tree.cpp.o: CMakeFiles/finmath.dir/flags.make
CMakeFiles/finmath.dir/src/cpp/OptionPricing/binomial_tree.cpp.o: /Users/shashank/finmath/src/cpp/OptionPricing/binomial_tree.cpp
CMakeFiles/finmath.dir/src/cpp/OptionPricing/binomial_tree.cpp.o: CMakeFiles/finmath.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/shashank/finmath/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/finmath.dir/src/cpp/OptionPricing/binomial_tree.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/finmath.dir/src/cpp/OptionPricing/binomial_tree.cpp.o -MF CMakeFiles/finmath.dir/src/cpp/OptionPricing/binomial_tree.cpp.o.d -o CMakeFiles/finmath.dir/src/cpp/OptionPricing/binomial_tree.cpp.o -c /Users/shashank/finmath/src/cpp/OptionPricing/binomial_tree.cpp

CMakeFiles/finmath.dir/src/cpp/OptionPricing/binomial_tree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/finmath.dir/src/cpp/OptionPricing/binomial_tree.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/shashank/finmath/src/cpp/OptionPricing/binomial_tree.cpp > CMakeFiles/finmath.dir/src/cpp/OptionPricing/binomial_tree.cpp.i

CMakeFiles/finmath.dir/src/cpp/OptionPricing/binomial_tree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/finmath.dir/src/cpp/OptionPricing/binomial_tree.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/shashank/finmath/src/cpp/OptionPricing/binomial_tree.cpp -o CMakeFiles/finmath.dir/src/cpp/OptionPricing/binomial_tree.cpp.s

CMakeFiles/finmath.dir/src/cpp/OptionPricing/black_scholes.cpp.o: CMakeFiles/finmath.dir/flags.make
CMakeFiles/finmath.dir/src/cpp/OptionPricing/black_scholes.cpp.o: /Users/shashank/finmath/src/cpp/OptionPricing/black_scholes.cpp
CMakeFiles/finmath.dir/src/cpp/OptionPricing/black_scholes.cpp.o: CMakeFiles/finmath.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/shashank/finmath/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/finmath.dir/src/cpp/OptionPricing/black_scholes.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/finmath.dir/src/cpp/OptionPricing/black_scholes.cpp.o -MF CMakeFiles/finmath.dir/src/cpp/OptionPricing/black_scholes.cpp.o.d -o CMakeFiles/finmath.dir/src/cpp/OptionPricing/black_scholes.cpp.o -c /Users/shashank/finmath/src/cpp/OptionPricing/black_scholes.cpp

CMakeFiles/finmath.dir/src/cpp/OptionPricing/black_scholes.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/finmath.dir/src/cpp/OptionPricing/black_scholes.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/shashank/finmath/src/cpp/OptionPricing/black_scholes.cpp > CMakeFiles/finmath.dir/src/cpp/OptionPricing/black_scholes.cpp.i

CMakeFiles/finmath.dir/src/cpp/OptionPricing/black_scholes.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/finmath.dir/src/cpp/OptionPricing/black_scholes.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/shashank/finmath/src/cpp/OptionPricing/black_scholes.cpp -o CMakeFiles/finmath.dir/src/cpp/OptionPricing/black_scholes.cpp.s

# Object files for target finmath
finmath_OBJECTS = \
"CMakeFiles/finmath.dir/src/cpp/Helper/helper.cpp.o" \
"CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/compound_interest.cpp.o" \
"CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/simple_interest.cpp.o" \
"CMakeFiles/finmath.dir/src/cpp/OptionPricing/binomial_tree.cpp.o" \
"CMakeFiles/finmath.dir/src/cpp/OptionPricing/black_scholes.cpp.o"

# External object files for target finmath
finmath_EXTERNAL_OBJECTS =

libfinmath.dylib: CMakeFiles/finmath.dir/src/cpp/Helper/helper.cpp.o
libfinmath.dylib: CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/compound_interest.cpp.o
libfinmath.dylib: CMakeFiles/finmath.dir/src/cpp/InterestAndAnnuities/simple_interest.cpp.o
libfinmath.dylib: CMakeFiles/finmath.dir/src/cpp/OptionPricing/binomial_tree.cpp.o
libfinmath.dylib: CMakeFiles/finmath.dir/src/cpp/OptionPricing/black_scholes.cpp.o
libfinmath.dylib: CMakeFiles/finmath.dir/build.make
libfinmath.dylib: CMakeFiles/finmath.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/shashank/finmath/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX shared library libfinmath.dylib"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/finmath.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/finmath.dir/build: libfinmath.dylib
.PHONY : CMakeFiles/finmath.dir/build

CMakeFiles/finmath.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/finmath.dir/cmake_clean.cmake
.PHONY : CMakeFiles/finmath.dir/clean

CMakeFiles/finmath.dir/depend:
	cd /Users/shashank/finmath/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/shashank/finmath /Users/shashank/finmath /Users/shashank/finmath/build /Users/shashank/finmath/build /Users/shashank/finmath/build/CMakeFiles/finmath.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/finmath.dir/depend

