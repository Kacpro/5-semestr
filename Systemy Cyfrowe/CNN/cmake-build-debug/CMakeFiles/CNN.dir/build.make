# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_COMMAND = /home/kacpro/Programy/clion-2017.3.4/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/kacpro/Programy/clion-2017.3.4/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/kacpro/5 semestr/Systemy Cyfrowe/CNN"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/kacpro/5 semestr/Systemy Cyfrowe/CNN/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/CNN.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CNN.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CNN.dir/flags.make

CMakeFiles/CNN.dir/main.c.o: CMakeFiles/CNN.dir/flags.make
CMakeFiles/CNN.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/kacpro/5 semestr/Systemy Cyfrowe/CNN/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/CNN.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/CNN.dir/main.c.o   -c "/home/kacpro/5 semestr/Systemy Cyfrowe/CNN/main.c"

CMakeFiles/CNN.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/CNN.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/kacpro/5 semestr/Systemy Cyfrowe/CNN/main.c" > CMakeFiles/CNN.dir/main.c.i

CMakeFiles/CNN.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/CNN.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/kacpro/5 semestr/Systemy Cyfrowe/CNN/main.c" -o CMakeFiles/CNN.dir/main.c.s

CMakeFiles/CNN.dir/main.c.o.requires:

.PHONY : CMakeFiles/CNN.dir/main.c.o.requires

CMakeFiles/CNN.dir/main.c.o.provides: CMakeFiles/CNN.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/CNN.dir/build.make CMakeFiles/CNN.dir/main.c.o.provides.build
.PHONY : CMakeFiles/CNN.dir/main.c.o.provides

CMakeFiles/CNN.dir/main.c.o.provides.build: CMakeFiles/CNN.dir/main.c.o


# Object files for target CNN
CNN_OBJECTS = \
"CMakeFiles/CNN.dir/main.c.o"

# External object files for target CNN
CNN_EXTERNAL_OBJECTS =

CNN: CMakeFiles/CNN.dir/main.c.o
CNN: CMakeFiles/CNN.dir/build.make
CNN: CMakeFiles/CNN.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/kacpro/5 semestr/Systemy Cyfrowe/CNN/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable CNN"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CNN.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CNN.dir/build: CNN

.PHONY : CMakeFiles/CNN.dir/build

CMakeFiles/CNN.dir/requires: CMakeFiles/CNN.dir/main.c.o.requires

.PHONY : CMakeFiles/CNN.dir/requires

CMakeFiles/CNN.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CNN.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CNN.dir/clean

CMakeFiles/CNN.dir/depend:
	cd "/home/kacpro/5 semestr/Systemy Cyfrowe/CNN/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/kacpro/5 semestr/Systemy Cyfrowe/CNN" "/home/kacpro/5 semestr/Systemy Cyfrowe/CNN" "/home/kacpro/5 semestr/Systemy Cyfrowe/CNN/cmake-build-debug" "/home/kacpro/5 semestr/Systemy Cyfrowe/CNN/cmake-build-debug" "/home/kacpro/5 semestr/Systemy Cyfrowe/CNN/cmake-build-debug/CMakeFiles/CNN.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/CNN.dir/depend

