# +----------------------------------------------+
# | project settings                             |
# +--------------------------------------------- +
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(ncc)
set(target_name ncc)

set(source_files
	src/main.cxx
)

set(lex_source
	src/scanner.lex
)

set(yacc_source
	src/parser.ypp
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
find_package(FLEX)
if (NOT FLEX_FOUND)
	message("Not found a proper flex program.")
	return ()
endif ()
message (STATUS "FLEX_VERSION: ${FLEX_VERSION}")

find_package(BISON)
if (NOT BISON_FOUND)
	message("Not found a proper bison program.")
	return ()
endif()
message (STATUS "BISON_VERSION: ${BISON_VERSION}")

# +----------------------------------------------+
# | Build executable                             |
# +--------------------------------------------- +
BISON_TARGET(MyParser ${yacc_source} ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(MyScanner ${lex_source}  ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(${target_name}
   ${source_files}
   ${BISON_MyParser_OUTPUTS}
   ${FLEX_MyScanner_OUTPUTS}
)