# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.31.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.31.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/coreyclark/Sites/Git/Classes/Projects/CS5393/Inclass/AIDev/websocket_client

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/coreyclark/Sites/Git/Classes/Projects/CS5393/Inclass/AIDev/websocket_client/build

# Include any dependencies generated for this target.
include CMakeFiles/ws_client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ws_client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ws_client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ws_client.dir/flags.make

CMakeFiles/ws_client.dir/codegen:
.PHONY : CMakeFiles/ws_client.dir/codegen

CMakeFiles/ws_client.dir/ws_client.cpp.o: CMakeFiles/ws_client.dir/flags.make
CMakeFiles/ws_client.dir/ws_client.cpp.o: /Users/coreyclark/Sites/Git/Classes/Projects/CS5393/Inclass/AIDev/websocket_client/ws_client.cpp
CMakeFiles/ws_client.dir/ws_client.cpp.o: CMakeFiles/ws_client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/coreyclark/Sites/Git/Classes/Projects/CS5393/Inclass/AIDev/websocket_client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ws_client.dir/ws_client.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ws_client.dir/ws_client.cpp.o -MF CMakeFiles/ws_client.dir/ws_client.cpp.o.d -o CMakeFiles/ws_client.dir/ws_client.cpp.o -c /Users/coreyclark/Sites/Git/Classes/Projects/CS5393/Inclass/AIDev/websocket_client/ws_client.cpp

CMakeFiles/ws_client.dir/ws_client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ws_client.dir/ws_client.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/coreyclark/Sites/Git/Classes/Projects/CS5393/Inclass/AIDev/websocket_client/ws_client.cpp > CMakeFiles/ws_client.dir/ws_client.cpp.i

CMakeFiles/ws_client.dir/ws_client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ws_client.dir/ws_client.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/coreyclark/Sites/Git/Classes/Projects/CS5393/Inclass/AIDev/websocket_client/ws_client.cpp -o CMakeFiles/ws_client.dir/ws_client.cpp.s

# Object files for target ws_client
ws_client_OBJECTS = \
"CMakeFiles/ws_client.dir/ws_client.cpp.o"

# External object files for target ws_client
ws_client_EXTERNAL_OBJECTS =

ws_client: CMakeFiles/ws_client.dir/ws_client.cpp.o
ws_client: CMakeFiles/ws_client.dir/build.make
ws_client: /usr/local/lib/libboost_system.dylib
ws_client: /usr/local/lib/libjsoncpp.26.dylib
ws_client: CMakeFiles/ws_client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/coreyclark/Sites/Git/Classes/Projects/CS5393/Inclass/AIDev/websocket_client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ws_client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ws_client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ws_client.dir/build: ws_client
.PHONY : CMakeFiles/ws_client.dir/build

CMakeFiles/ws_client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ws_client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ws_client.dir/clean

CMakeFiles/ws_client.dir/depend:
	cd /Users/coreyclark/Sites/Git/Classes/Projects/CS5393/Inclass/AIDev/websocket_client/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/coreyclark/Sites/Git/Classes/Projects/CS5393/Inclass/AIDev/websocket_client /Users/coreyclark/Sites/Git/Classes/Projects/CS5393/Inclass/AIDev/websocket_client /Users/coreyclark/Sites/Git/Classes/Projects/CS5393/Inclass/AIDev/websocket_client/build /Users/coreyclark/Sites/Git/Classes/Projects/CS5393/Inclass/AIDev/websocket_client/build /Users/coreyclark/Sites/Git/Classes/Projects/CS5393/Inclass/AIDev/websocket_client/build/CMakeFiles/ws_client.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/ws_client.dir/depend

