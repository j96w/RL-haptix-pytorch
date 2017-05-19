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
include CMakeFiles/armctl.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/armctl.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/armctl.dir/flags.make

CMakeFiles/armctl.dir/hx_ctl.c.o: CMakeFiles/armctl.dir/flags.make
CMakeFiles/armctl.dir/hx_ctl.c.o: ../hx_ctl.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jeremywang/haptix_controller/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/armctl.dir/hx_ctl.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/armctl.dir/hx_ctl.c.o   -c /home/jeremywang/haptix_controller/hx_ctl.c

CMakeFiles/armctl.dir/hx_ctl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/armctl.dir/hx_ctl.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/jeremywang/haptix_controller/hx_ctl.c > CMakeFiles/armctl.dir/hx_ctl.c.i

CMakeFiles/armctl.dir/hx_ctl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/armctl.dir/hx_ctl.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/jeremywang/haptix_controller/hx_ctl.c -o CMakeFiles/armctl.dir/hx_ctl.c.s

CMakeFiles/armctl.dir/hx_ctl.c.o.requires:
.PHONY : CMakeFiles/armctl.dir/hx_ctl.c.o.requires

CMakeFiles/armctl.dir/hx_ctl.c.o.provides: CMakeFiles/armctl.dir/hx_ctl.c.o.requires
	$(MAKE) -f CMakeFiles/armctl.dir/build.make CMakeFiles/armctl.dir/hx_ctl.c.o.provides.build
.PHONY : CMakeFiles/armctl.dir/hx_ctl.c.o.provides

CMakeFiles/armctl.dir/hx_ctl.c.o.provides.build: CMakeFiles/armctl.dir/hx_ctl.c.o

# Object files for target armctl
armctl_OBJECTS = \
"CMakeFiles/armctl.dir/hx_ctl.c.o"

# External object files for target armctl
armctl_EXTERNAL_OBJECTS =

libarmctl.so: CMakeFiles/armctl.dir/hx_ctl.c.o
libarmctl.so: CMakeFiles/armctl.dir/build.make
libarmctl.so: /usr/lib/x86_64-linux-gnu/libhaptix-comm.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libhaptix-msgs.a
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_client.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_gui.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_sensors.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_rendering.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_player.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_physics.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_ode.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_transport.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_msgs.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_util.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_common.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_gimpact.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_opcode.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_opende_ou.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_math.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_ccd.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_thread.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_signals.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_system.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_program_options.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_regex.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_iostreams.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libpthread.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libprotobuf.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libsdformat.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libignition-math2.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libOgreMain.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_thread.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_system.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_atomic.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libpthread.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libOgreTerrain.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libOgrePaging.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libignition-math2.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_client.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_gui.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_sensors.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_rendering.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_player.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_physics.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_ode.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_transport.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_msgs.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_util.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_common.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_gimpact.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_opcode.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_opende_ou.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_math.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libgazebo_ccd.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_thread.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_signals.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_system.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_program_options.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_regex.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_iostreams.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libpthread.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libprotobuf.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libsdformat.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libOgreMain.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_atomic.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libOgreTerrain.so
libarmctl.so: /usr/lib/x86_64-linux-gnu/libOgrePaging.so
libarmctl.so: CMakeFiles/armctl.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C shared library libarmctl.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/armctl.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/armctl.dir/build: libarmctl.so
.PHONY : CMakeFiles/armctl.dir/build

CMakeFiles/armctl.dir/requires: CMakeFiles/armctl.dir/hx_ctl.c.o.requires
.PHONY : CMakeFiles/armctl.dir/requires

CMakeFiles/armctl.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/armctl.dir/cmake_clean.cmake
.PHONY : CMakeFiles/armctl.dir/clean

CMakeFiles/armctl.dir/depend:
	cd /home/jeremywang/haptix_controller/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jeremywang/haptix_controller /home/jeremywang/haptix_controller /home/jeremywang/haptix_controller/build /home/jeremywang/haptix_controller/build /home/jeremywang/haptix_controller/build/CMakeFiles/armctl.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/armctl.dir/depend
