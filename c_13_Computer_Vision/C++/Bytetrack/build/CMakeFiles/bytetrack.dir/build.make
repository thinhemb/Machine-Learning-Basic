# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/build

# Include any dependencies generated for this target.
include CMakeFiles/bytetrack.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/bytetrack.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/bytetrack.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bytetrack.dir/flags.make

CMakeFiles/bytetrack.dir/src/bytetrack.cpp.o: CMakeFiles/bytetrack.dir/flags.make
CMakeFiles/bytetrack.dir/src/bytetrack.cpp.o: ../src/bytetrack.cpp
CMakeFiles/bytetrack.dir/src/bytetrack.cpp.o: CMakeFiles/bytetrack.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/bytetrack.dir/src/bytetrack.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/bytetrack.dir/src/bytetrack.cpp.o -MF CMakeFiles/bytetrack.dir/src/bytetrack.cpp.o.d -o CMakeFiles/bytetrack.dir/src/bytetrack.cpp.o -c /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/bytetrack.cpp

CMakeFiles/bytetrack.dir/src/bytetrack.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bytetrack.dir/src/bytetrack.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/bytetrack.cpp > CMakeFiles/bytetrack.dir/src/bytetrack.cpp.i

CMakeFiles/bytetrack.dir/src/bytetrack.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bytetrack.dir/src/bytetrack.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/bytetrack.cpp -o CMakeFiles/bytetrack.dir/src/bytetrack.cpp.s

CMakeFiles/bytetrack.dir/src/BYTETracker.cpp.o: CMakeFiles/bytetrack.dir/flags.make
CMakeFiles/bytetrack.dir/src/BYTETracker.cpp.o: ../src/BYTETracker.cpp
CMakeFiles/bytetrack.dir/src/BYTETracker.cpp.o: CMakeFiles/bytetrack.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/bytetrack.dir/src/BYTETracker.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/bytetrack.dir/src/BYTETracker.cpp.o -MF CMakeFiles/bytetrack.dir/src/BYTETracker.cpp.o.d -o CMakeFiles/bytetrack.dir/src/BYTETracker.cpp.o -c /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/BYTETracker.cpp

CMakeFiles/bytetrack.dir/src/BYTETracker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bytetrack.dir/src/BYTETracker.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/BYTETracker.cpp > CMakeFiles/bytetrack.dir/src/BYTETracker.cpp.i

CMakeFiles/bytetrack.dir/src/BYTETracker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bytetrack.dir/src/BYTETracker.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/BYTETracker.cpp -o CMakeFiles/bytetrack.dir/src/BYTETracker.cpp.s

CMakeFiles/bytetrack.dir/src/kalmanFilter.cpp.o: CMakeFiles/bytetrack.dir/flags.make
CMakeFiles/bytetrack.dir/src/kalmanFilter.cpp.o: ../src/kalmanFilter.cpp
CMakeFiles/bytetrack.dir/src/kalmanFilter.cpp.o: CMakeFiles/bytetrack.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/bytetrack.dir/src/kalmanFilter.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/bytetrack.dir/src/kalmanFilter.cpp.o -MF CMakeFiles/bytetrack.dir/src/kalmanFilter.cpp.o.d -o CMakeFiles/bytetrack.dir/src/kalmanFilter.cpp.o -c /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/kalmanFilter.cpp

CMakeFiles/bytetrack.dir/src/kalmanFilter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bytetrack.dir/src/kalmanFilter.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/kalmanFilter.cpp > CMakeFiles/bytetrack.dir/src/kalmanFilter.cpp.i

CMakeFiles/bytetrack.dir/src/kalmanFilter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bytetrack.dir/src/kalmanFilter.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/kalmanFilter.cpp -o CMakeFiles/bytetrack.dir/src/kalmanFilter.cpp.s

CMakeFiles/bytetrack.dir/src/lapjv.cpp.o: CMakeFiles/bytetrack.dir/flags.make
CMakeFiles/bytetrack.dir/src/lapjv.cpp.o: ../src/lapjv.cpp
CMakeFiles/bytetrack.dir/src/lapjv.cpp.o: CMakeFiles/bytetrack.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/bytetrack.dir/src/lapjv.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/bytetrack.dir/src/lapjv.cpp.o -MF CMakeFiles/bytetrack.dir/src/lapjv.cpp.o.d -o CMakeFiles/bytetrack.dir/src/lapjv.cpp.o -c /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/lapjv.cpp

CMakeFiles/bytetrack.dir/src/lapjv.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bytetrack.dir/src/lapjv.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/lapjv.cpp > CMakeFiles/bytetrack.dir/src/lapjv.cpp.i

CMakeFiles/bytetrack.dir/src/lapjv.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bytetrack.dir/src/lapjv.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/lapjv.cpp -o CMakeFiles/bytetrack.dir/src/lapjv.cpp.s

CMakeFiles/bytetrack.dir/src/STrack.cpp.o: CMakeFiles/bytetrack.dir/flags.make
CMakeFiles/bytetrack.dir/src/STrack.cpp.o: ../src/STrack.cpp
CMakeFiles/bytetrack.dir/src/STrack.cpp.o: CMakeFiles/bytetrack.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/bytetrack.dir/src/STrack.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/bytetrack.dir/src/STrack.cpp.o -MF CMakeFiles/bytetrack.dir/src/STrack.cpp.o.d -o CMakeFiles/bytetrack.dir/src/STrack.cpp.o -c /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/STrack.cpp

CMakeFiles/bytetrack.dir/src/STrack.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bytetrack.dir/src/STrack.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/STrack.cpp > CMakeFiles/bytetrack.dir/src/STrack.cpp.i

CMakeFiles/bytetrack.dir/src/STrack.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bytetrack.dir/src/STrack.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/STrack.cpp -o CMakeFiles/bytetrack.dir/src/STrack.cpp.s

CMakeFiles/bytetrack.dir/src/utils.cpp.o: CMakeFiles/bytetrack.dir/flags.make
CMakeFiles/bytetrack.dir/src/utils.cpp.o: ../src/utils.cpp
CMakeFiles/bytetrack.dir/src/utils.cpp.o: CMakeFiles/bytetrack.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/bytetrack.dir/src/utils.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/bytetrack.dir/src/utils.cpp.o -MF CMakeFiles/bytetrack.dir/src/utils.cpp.o.d -o CMakeFiles/bytetrack.dir/src/utils.cpp.o -c /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/utils.cpp

CMakeFiles/bytetrack.dir/src/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bytetrack.dir/src/utils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/utils.cpp > CMakeFiles/bytetrack.dir/src/utils.cpp.i

CMakeFiles/bytetrack.dir/src/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bytetrack.dir/src/utils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/utils.cpp -o CMakeFiles/bytetrack.dir/src/utils.cpp.s

CMakeFiles/bytetrack.dir/src/yolov4_tiny.cpp.o: CMakeFiles/bytetrack.dir/flags.make
CMakeFiles/bytetrack.dir/src/yolov4_tiny.cpp.o: ../src/yolov4_tiny.cpp
CMakeFiles/bytetrack.dir/src/yolov4_tiny.cpp.o: CMakeFiles/bytetrack.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/bytetrack.dir/src/yolov4_tiny.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/bytetrack.dir/src/yolov4_tiny.cpp.o -MF CMakeFiles/bytetrack.dir/src/yolov4_tiny.cpp.o.d -o CMakeFiles/bytetrack.dir/src/yolov4_tiny.cpp.o -c /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/yolov4_tiny.cpp

CMakeFiles/bytetrack.dir/src/yolov4_tiny.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bytetrack.dir/src/yolov4_tiny.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/yolov4_tiny.cpp > CMakeFiles/bytetrack.dir/src/yolov4_tiny.cpp.i

CMakeFiles/bytetrack.dir/src/yolov4_tiny.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bytetrack.dir/src/yolov4_tiny.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/src/yolov4_tiny.cpp -o CMakeFiles/bytetrack.dir/src/yolov4_tiny.cpp.s

# Object files for target bytetrack
bytetrack_OBJECTS = \
"CMakeFiles/bytetrack.dir/src/bytetrack.cpp.o" \
"CMakeFiles/bytetrack.dir/src/BYTETracker.cpp.o" \
"CMakeFiles/bytetrack.dir/src/kalmanFilter.cpp.o" \
"CMakeFiles/bytetrack.dir/src/lapjv.cpp.o" \
"CMakeFiles/bytetrack.dir/src/STrack.cpp.o" \
"CMakeFiles/bytetrack.dir/src/utils.cpp.o" \
"CMakeFiles/bytetrack.dir/src/yolov4_tiny.cpp.o"

# External object files for target bytetrack
bytetrack_EXTERNAL_OBJECTS =

bytetrack: CMakeFiles/bytetrack.dir/src/bytetrack.cpp.o
bytetrack: CMakeFiles/bytetrack.dir/src/BYTETracker.cpp.o
bytetrack: CMakeFiles/bytetrack.dir/src/kalmanFilter.cpp.o
bytetrack: CMakeFiles/bytetrack.dir/src/lapjv.cpp.o
bytetrack: CMakeFiles/bytetrack.dir/src/STrack.cpp.o
bytetrack: CMakeFiles/bytetrack.dir/src/utils.cpp.o
bytetrack: CMakeFiles/bytetrack.dir/src/yolov4_tiny.cpp.o
bytetrack: CMakeFiles/bytetrack.dir/build.make
bytetrack: CMakeFiles/bytetrack.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable bytetrack"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bytetrack.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bytetrack.dir/build: bytetrack
.PHONY : CMakeFiles/bytetrack.dir/build

CMakeFiles/bytetrack.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bytetrack.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bytetrack.dir/clean

CMakeFiles/bytetrack.dir/depend:
	cd /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/build /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/build /home/thinhdo/WorkSpace/thinhdv_core/Bytetrack/build/CMakeFiles/bytetrack.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bytetrack.dir/depend
