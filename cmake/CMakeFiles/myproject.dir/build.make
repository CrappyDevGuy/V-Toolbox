# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dunkhey/workspace/CXX/V-Toolbox

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dunkhey/workspace/CXX/V-Toolbox/cmake

# Include any dependencies generated for this target.
include CMakeFiles/myproject.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/myproject.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/myproject.dir/flags.make

CMakeFiles/myproject.dir/src/core/instance.cpp.o: CMakeFiles/myproject.dir/flags.make
CMakeFiles/myproject.dir/src/core/instance.cpp.o: ../src/core/instance.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dunkhey/workspace/CXX/V-Toolbox/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/myproject.dir/src/core/instance.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myproject.dir/src/core/instance.cpp.o -c /home/dunkhey/workspace/CXX/V-Toolbox/src/core/instance.cpp

CMakeFiles/myproject.dir/src/core/instance.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myproject.dir/src/core/instance.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dunkhey/workspace/CXX/V-Toolbox/src/core/instance.cpp > CMakeFiles/myproject.dir/src/core/instance.cpp.i

CMakeFiles/myproject.dir/src/core/instance.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myproject.dir/src/core/instance.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dunkhey/workspace/CXX/V-Toolbox/src/core/instance.cpp -o CMakeFiles/myproject.dir/src/core/instance.cpp.s

CMakeFiles/myproject.dir/src/main.cpp.o: CMakeFiles/myproject.dir/flags.make
CMakeFiles/myproject.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dunkhey/workspace/CXX/V-Toolbox/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/myproject.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myproject.dir/src/main.cpp.o -c /home/dunkhey/workspace/CXX/V-Toolbox/src/main.cpp

CMakeFiles/myproject.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myproject.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dunkhey/workspace/CXX/V-Toolbox/src/main.cpp > CMakeFiles/myproject.dir/src/main.cpp.i

CMakeFiles/myproject.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myproject.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dunkhey/workspace/CXX/V-Toolbox/src/main.cpp -o CMakeFiles/myproject.dir/src/main.cpp.s

# Object files for target myproject
myproject_OBJECTS = \
"CMakeFiles/myproject.dir/src/core/instance.cpp.o" \
"CMakeFiles/myproject.dir/src/main.cpp.o"

# External object files for target myproject
myproject_EXTERNAL_OBJECTS =

myproject: CMakeFiles/myproject.dir/src/core/instance.cpp.o
myproject: CMakeFiles/myproject.dir/src/main.cpp.o
myproject: CMakeFiles/myproject.dir/build.make
myproject: CMakeFiles/myproject.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dunkhey/workspace/CXX/V-Toolbox/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable myproject"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/myproject.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/myproject.dir/build: myproject

.PHONY : CMakeFiles/myproject.dir/build

CMakeFiles/myproject.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/myproject.dir/cmake_clean.cmake
.PHONY : CMakeFiles/myproject.dir/clean

CMakeFiles/myproject.dir/depend:
	cd /home/dunkhey/workspace/CXX/V-Toolbox/cmake && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dunkhey/workspace/CXX/V-Toolbox /home/dunkhey/workspace/CXX/V-Toolbox /home/dunkhey/workspace/CXX/V-Toolbox/cmake /home/dunkhey/workspace/CXX/V-Toolbox/cmake /home/dunkhey/workspace/CXX/V-Toolbox/cmake/CMakeFiles/myproject.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/myproject.dir/depend

