# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /tmp/tmp.Wv2m9H2AB0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.Wv2m9H2AB0/cmake-build-debug-19216872225

# Include any dependencies generated for this target.
include CMakeFiles/plib.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/plib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/plib.dir/flags.make

CMakeFiles/plib.dir/clib.c.o: CMakeFiles/plib.dir/flags.make
CMakeFiles/plib.dir/clib.c.o: ../clib.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.Wv2m9H2AB0/cmake-build-debug-19216872225/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/plib.dir/clib.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/plib.dir/clib.c.o   -c /tmp/tmp.Wv2m9H2AB0/clib.c

CMakeFiles/plib.dir/clib.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/plib.dir/clib.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.Wv2m9H2AB0/clib.c > CMakeFiles/plib.dir/clib.c.i

CMakeFiles/plib.dir/clib.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/plib.dir/clib.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.Wv2m9H2AB0/clib.c -o CMakeFiles/plib.dir/clib.c.s

CMakeFiles/plib.dir/mainplib.cpp.o: CMakeFiles/plib.dir/flags.make
CMakeFiles/plib.dir/mainplib.cpp.o: ../mainplib.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.Wv2m9H2AB0/cmake-build-debug-19216872225/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/plib.dir/mainplib.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/plib.dir/mainplib.cpp.o -c /tmp/tmp.Wv2m9H2AB0/mainplib.cpp

CMakeFiles/plib.dir/mainplib.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/plib.dir/mainplib.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.Wv2m9H2AB0/mainplib.cpp > CMakeFiles/plib.dir/mainplib.cpp.i

CMakeFiles/plib.dir/mainplib.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/plib.dir/mainplib.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.Wv2m9H2AB0/mainplib.cpp -o CMakeFiles/plib.dir/mainplib.cpp.s

CMakeFiles/plib.dir/pudp.cpp.o: CMakeFiles/plib.dir/flags.make
CMakeFiles/plib.dir/pudp.cpp.o: ../pudp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.Wv2m9H2AB0/cmake-build-debug-19216872225/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/plib.dir/pudp.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/plib.dir/pudp.cpp.o -c /tmp/tmp.Wv2m9H2AB0/pudp.cpp

CMakeFiles/plib.dir/pudp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/plib.dir/pudp.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.Wv2m9H2AB0/pudp.cpp > CMakeFiles/plib.dir/pudp.cpp.i

CMakeFiles/plib.dir/pudp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/plib.dir/pudp.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.Wv2m9H2AB0/pudp.cpp -o CMakeFiles/plib.dir/pudp.cpp.s

CMakeFiles/plib.dir/plib.cpp.o: CMakeFiles/plib.dir/flags.make
CMakeFiles/plib.dir/plib.cpp.o: ../plib.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.Wv2m9H2AB0/cmake-build-debug-19216872225/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/plib.dir/plib.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/plib.dir/plib.cpp.o -c /tmp/tmp.Wv2m9H2AB0/plib.cpp

CMakeFiles/plib.dir/plib.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/plib.dir/plib.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.Wv2m9H2AB0/plib.cpp > CMakeFiles/plib.dir/plib.cpp.i

CMakeFiles/plib.dir/plib.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/plib.dir/plib.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.Wv2m9H2AB0/plib.cpp -o CMakeFiles/plib.dir/plib.cpp.s

CMakeFiles/plib.dir/pmysql.cpp.o: CMakeFiles/plib.dir/flags.make
CMakeFiles/plib.dir/pmysql.cpp.o: ../pmysql.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.Wv2m9H2AB0/cmake-build-debug-19216872225/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/plib.dir/pmysql.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/plib.dir/pmysql.cpp.o -c /tmp/tmp.Wv2m9H2AB0/pmysql.cpp

CMakeFiles/plib.dir/pmysql.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/plib.dir/pmysql.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.Wv2m9H2AB0/pmysql.cpp > CMakeFiles/plib.dir/pmysql.cpp.i

CMakeFiles/plib.dir/pmysql.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/plib.dir/pmysql.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.Wv2m9H2AB0/pmysql.cpp -o CMakeFiles/plib.dir/pmysql.cpp.s

CMakeFiles/plib.dir/pxml.cpp.o: CMakeFiles/plib.dir/flags.make
CMakeFiles/plib.dir/pxml.cpp.o: ../pxml.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.Wv2m9H2AB0/cmake-build-debug-19216872225/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/plib.dir/pxml.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/plib.dir/pxml.cpp.o -c /tmp/tmp.Wv2m9H2AB0/pxml.cpp

CMakeFiles/plib.dir/pxml.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/plib.dir/pxml.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.Wv2m9H2AB0/pxml.cpp > CMakeFiles/plib.dir/pxml.cpp.i

CMakeFiles/plib.dir/pxml.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/plib.dir/pxml.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.Wv2m9H2AB0/pxml.cpp -o CMakeFiles/plib.dir/pxml.cpp.s

CMakeFiles/plib.dir/tinystr.cpp.o: CMakeFiles/plib.dir/flags.make
CMakeFiles/plib.dir/tinystr.cpp.o: ../tinystr.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.Wv2m9H2AB0/cmake-build-debug-19216872225/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/plib.dir/tinystr.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/plib.dir/tinystr.cpp.o -c /tmp/tmp.Wv2m9H2AB0/tinystr.cpp

CMakeFiles/plib.dir/tinystr.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/plib.dir/tinystr.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.Wv2m9H2AB0/tinystr.cpp > CMakeFiles/plib.dir/tinystr.cpp.i

CMakeFiles/plib.dir/tinystr.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/plib.dir/tinystr.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.Wv2m9H2AB0/tinystr.cpp -o CMakeFiles/plib.dir/tinystr.cpp.s

CMakeFiles/plib.dir/tinyxml.cpp.o: CMakeFiles/plib.dir/flags.make
CMakeFiles/plib.dir/tinyxml.cpp.o: ../tinyxml.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.Wv2m9H2AB0/cmake-build-debug-19216872225/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/plib.dir/tinyxml.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/plib.dir/tinyxml.cpp.o -c /tmp/tmp.Wv2m9H2AB0/tinyxml.cpp

CMakeFiles/plib.dir/tinyxml.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/plib.dir/tinyxml.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.Wv2m9H2AB0/tinyxml.cpp > CMakeFiles/plib.dir/tinyxml.cpp.i

CMakeFiles/plib.dir/tinyxml.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/plib.dir/tinyxml.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.Wv2m9H2AB0/tinyxml.cpp -o CMakeFiles/plib.dir/tinyxml.cpp.s

CMakeFiles/plib.dir/tinyxmlerror.cpp.o: CMakeFiles/plib.dir/flags.make
CMakeFiles/plib.dir/tinyxmlerror.cpp.o: ../tinyxmlerror.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.Wv2m9H2AB0/cmake-build-debug-19216872225/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/plib.dir/tinyxmlerror.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/plib.dir/tinyxmlerror.cpp.o -c /tmp/tmp.Wv2m9H2AB0/tinyxmlerror.cpp

CMakeFiles/plib.dir/tinyxmlerror.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/plib.dir/tinyxmlerror.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.Wv2m9H2AB0/tinyxmlerror.cpp > CMakeFiles/plib.dir/tinyxmlerror.cpp.i

CMakeFiles/plib.dir/tinyxmlerror.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/plib.dir/tinyxmlerror.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.Wv2m9H2AB0/tinyxmlerror.cpp -o CMakeFiles/plib.dir/tinyxmlerror.cpp.s

CMakeFiles/plib.dir/tinyxmlparser.cpp.o: CMakeFiles/plib.dir/flags.make
CMakeFiles/plib.dir/tinyxmlparser.cpp.o: ../tinyxmlparser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.Wv2m9H2AB0/cmake-build-debug-19216872225/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/plib.dir/tinyxmlparser.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/plib.dir/tinyxmlparser.cpp.o -c /tmp/tmp.Wv2m9H2AB0/tinyxmlparser.cpp

CMakeFiles/plib.dir/tinyxmlparser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/plib.dir/tinyxmlparser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.Wv2m9H2AB0/tinyxmlparser.cpp > CMakeFiles/plib.dir/tinyxmlparser.cpp.i

CMakeFiles/plib.dir/tinyxmlparser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/plib.dir/tinyxmlparser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.Wv2m9H2AB0/tinyxmlparser.cpp -o CMakeFiles/plib.dir/tinyxmlparser.cpp.s

CMakeFiles/plib.dir/ptcp.cpp.o: CMakeFiles/plib.dir/flags.make
CMakeFiles/plib.dir/ptcp.cpp.o: ../ptcp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.Wv2m9H2AB0/cmake-build-debug-19216872225/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/plib.dir/ptcp.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/plib.dir/ptcp.cpp.o -c /tmp/tmp.Wv2m9H2AB0/ptcp.cpp

CMakeFiles/plib.dir/ptcp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/plib.dir/ptcp.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.Wv2m9H2AB0/ptcp.cpp > CMakeFiles/plib.dir/ptcp.cpp.i

CMakeFiles/plib.dir/ptcp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/plib.dir/ptcp.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.Wv2m9H2AB0/ptcp.cpp -o CMakeFiles/plib.dir/ptcp.cpp.s

# Object files for target plib
plib_OBJECTS = \
"CMakeFiles/plib.dir/clib.c.o" \
"CMakeFiles/plib.dir/mainplib.cpp.o" \
"CMakeFiles/plib.dir/pudp.cpp.o" \
"CMakeFiles/plib.dir/plib.cpp.o" \
"CMakeFiles/plib.dir/pmysql.cpp.o" \
"CMakeFiles/plib.dir/pxml.cpp.o" \
"CMakeFiles/plib.dir/tinystr.cpp.o" \
"CMakeFiles/plib.dir/tinyxml.cpp.o" \
"CMakeFiles/plib.dir/tinyxmlerror.cpp.o" \
"CMakeFiles/plib.dir/tinyxmlparser.cpp.o" \
"CMakeFiles/plib.dir/ptcp.cpp.o"

# External object files for target plib
plib_EXTERNAL_OBJECTS =

plib: CMakeFiles/plib.dir/clib.c.o
plib: CMakeFiles/plib.dir/mainplib.cpp.o
plib: CMakeFiles/plib.dir/pudp.cpp.o
plib: CMakeFiles/plib.dir/plib.cpp.o
plib: CMakeFiles/plib.dir/pmysql.cpp.o
plib: CMakeFiles/plib.dir/pxml.cpp.o
plib: CMakeFiles/plib.dir/tinystr.cpp.o
plib: CMakeFiles/plib.dir/tinyxml.cpp.o
plib: CMakeFiles/plib.dir/tinyxmlerror.cpp.o
plib: CMakeFiles/plib.dir/tinyxmlparser.cpp.o
plib: CMakeFiles/plib.dir/ptcp.cpp.o
plib: CMakeFiles/plib.dir/build.make
plib: CMakeFiles/plib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.Wv2m9H2AB0/cmake-build-debug-19216872225/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable plib"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/plib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/plib.dir/build: plib

.PHONY : CMakeFiles/plib.dir/build

CMakeFiles/plib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/plib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/plib.dir/clean

CMakeFiles/plib.dir/depend:
	cd /tmp/tmp.Wv2m9H2AB0/cmake-build-debug-19216872225 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.Wv2m9H2AB0 /tmp/tmp.Wv2m9H2AB0 /tmp/tmp.Wv2m9H2AB0/cmake-build-debug-19216872225 /tmp/tmp.Wv2m9H2AB0/cmake-build-debug-19216872225 /tmp/tmp.Wv2m9H2AB0/cmake-build-debug-19216872225/CMakeFiles/plib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/plib.dir/depend

