# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/jeremywang/haptix_controller

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jeremywang/haptix_controller/build

# Include any dependencies generated for this target.
include CMakeFiles/aaactl.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/aaactl.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/aaactl.dir/flags.make

CMakeFiles/aaactl.dir/hx_ctl.c.o: CMakeFiles/aaactl.dir/flags.make
CMakeFiles/aaactl.dir/hx_ctl.c.o: ../hx_ctl.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jeremywang/haptix_controller/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/aaactl.dir/hx_ctl.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/aaactl.dir/hx_ctl.c.o   -c /home/jeremywang/haptix_controller/hx_ctl.c

CMakeFiles/aaactl.dir/hx_ctl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/aaactl.dir/hx_ctl.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/jeremywang/haptix_controller/hx_ctl.c > CMakeFiles/aaactl.dir/hx_ctl.c.i

CMakeFiles/aaactl.dir/hx_ctl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/aaactl.dir/hx_ctl.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/jeremywang/haptix_controller/hx_ctl.c -o CMakeFiles/aaactl.dir/hx_ctl.c.s

CMakeFiles/aaactl.dir/hx_ctl.c.o.requires:
.PHONY : CMakeFiles/aaactl.dir/hx_ctl.c.o.requires

CMakeFiles/aaactl.dir/hx_ctl.c.o.provides: CMakeFiles/aaactl.dir/hx_ctl.c.o.requires
	$(MAKE) -f CMakeFiles/aaactl.dir/build.make CMakeFiles/aaactl.dir/hx_ctl.c.o.provides.build
.PHONY : CMakeFiles/aaactl.dir/hx_ctl.c.o.provides

CMakeFiles/aaactl.dir/hx_ctl.c.o.provides.build: CMakeFiles/aaactl.dir/hx_ctl.c.o

# Object files for target aaactl
aaactl_OBJECTS = \
"CMakeFiles/aaactl.dir/hx_ctl.c.o"

# External object files for target aaactl
aaactl_EXTERNAL_OBJECTS =

libaaactl.so: CMakeFiles/aaactl.dir/hx_ctl.c.o
libaaactl.so: CMakeFiles/aaactl.dir/build.make
libaaactl.so: /usr/lib/x86_64-linux-gnu/libhaptix-comm.so
libaaactl.so: /usr/lib/x86_64-linux-gnu/libhaptix-msgs.a
libaaactl.so: CMakeFiles/aaactl.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C shared library libaaactl.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/aaactl.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/aaactl.dir/build: libaaactl.so
.PHONY : CMakeFiles/aaactl.dir/build

CMakeFiles/aaactl.dir/requires: CMakeFiles/aaactl.dir/hx_ctl.c.o.requires
.PHONY : CMakeFiles/aaactl.dir/requires

CMakeFiles/aaactl.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/aaactl.dir/cmake_clean.cmake
.PHONY : CMakeFiles/aaactl.dir/clean

CMakeFiles/aaactl.dir/depend:
	cd /home/jeremywang/haptix_controller/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jeremywang/haptix_controller /home/jeremywang/haptix_controller /home/jeremywang/haptix_controller/build /home/jeremywang/haptix_controller/build /home/jeremywang/haptix_controller/build/CMakeFiles/aaactl.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/aaactl.dir/depend
