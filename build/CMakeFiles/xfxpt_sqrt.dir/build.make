# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /static/workspaces/gitworkspace/SXLAL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /static/workspaces/gitworkspace/SXLAL/build

# Include any dependencies generated for this target.
include CMakeFiles/xfxpt_sqrt.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/xfxpt_sqrt.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/xfxpt_sqrt.dir/flags.make

CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.o: CMakeFiles/xfxpt_sqrt.dir/flags.make
CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.o: ../examples/c/xsqrt/xfxpt_sqrt.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/static/workspaces/gitworkspace/SXLAL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.o -c /static/workspaces/gitworkspace/SXLAL/examples/c/xsqrt/xfxpt_sqrt.cpp

CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /static/workspaces/gitworkspace/SXLAL/examples/c/xsqrt/xfxpt_sqrt.cpp > CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.i

CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /static/workspaces/gitworkspace/SXLAL/examples/c/xsqrt/xfxpt_sqrt.cpp -o CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.s

CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.o.requires:

.PHONY : CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.o.requires

CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.o.provides: CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.o.requires
	$(MAKE) -f CMakeFiles/xfxpt_sqrt.dir/build.make CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.o.provides.build
.PHONY : CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.o.provides

CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.o.provides.build: CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.o


# Object files for target xfxpt_sqrt
xfxpt_sqrt_OBJECTS = \
"CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.o"

# External object files for target xfxpt_sqrt
xfxpt_sqrt_EXTERNAL_OBJECTS =

xfxpt_sqrt: CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.o
xfxpt_sqrt: CMakeFiles/xfxpt_sqrt.dir/build.make
xfxpt_sqrt: CMakeFiles/xfxpt_sqrt.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/static/workspaces/gitworkspace/SXLAL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable xfxpt_sqrt"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/xfxpt_sqrt.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/xfxpt_sqrt.dir/build: xfxpt_sqrt

.PHONY : CMakeFiles/xfxpt_sqrt.dir/build

CMakeFiles/xfxpt_sqrt.dir/requires: CMakeFiles/xfxpt_sqrt.dir/examples/c/xsqrt/xfxpt_sqrt.cpp.o.requires

.PHONY : CMakeFiles/xfxpt_sqrt.dir/requires

CMakeFiles/xfxpt_sqrt.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/xfxpt_sqrt.dir/cmake_clean.cmake
.PHONY : CMakeFiles/xfxpt_sqrt.dir/clean

CMakeFiles/xfxpt_sqrt.dir/depend:
	cd /static/workspaces/gitworkspace/SXLAL/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /static/workspaces/gitworkspace/SXLAL /static/workspaces/gitworkspace/SXLAL /static/workspaces/gitworkspace/SXLAL/build /static/workspaces/gitworkspace/SXLAL/build /static/workspaces/gitworkspace/SXLAL/build/CMakeFiles/xfxpt_sqrt.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/xfxpt_sqrt.dir/depend

