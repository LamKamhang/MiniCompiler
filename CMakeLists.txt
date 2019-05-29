# +----------------------------------------------+
# | project settings                             |
# +--------------------------------------------- +
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(ncc)
set(target_name ncc)

set(source_files
	src/main.cxx
	src/parser.cpp
	src/scanner.cpp
	src/ast/ast.cc
	src/ast/include/json/jsoncpp.cc
)

# +----------------------------------------------+
# | Complilation flags                           |
# +--------------------------------------------- +
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
if (PROFILE_FLAG)
	message(STATUS "profiling")
	if (CMAKE_COMPILER_IS_GNUCXX)
		# set(CMAKE_EXE_LINKER_FLAGS "-pthread")
		set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g -pg")
		set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -pg")
	endif (CMAKE_COMPILER_IS_GNUCXX)
else (PROFILE_FLAG)
	if (CMAKE_COMPILER_IS_GNUCXX)
		# set(CMAKE_EXE_LINKER_FLAGS "-pthread")
		set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g")
		set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")
	endif (CMAKE_COMPILER_IS_GNUCXX)
endif (PROFILE_FLAG)

# +----------------------------------------------+
# | Third Party Libray                           |
# +--------------------------------------------- +
include_directories(${PROJECT_SOURCE_DIR}/src)
message("${PROJECT_SOURCE_DIR}")
# +----------------------------------------------+
# | Build executable                             |
# +--------------------------------------------- +
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(${target_name}
   ${source_files}
)