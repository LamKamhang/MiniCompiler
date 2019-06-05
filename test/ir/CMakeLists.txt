cmake_minimum_required(VERSION 2.8)

project(ir_test)
set(target ir_test)

set(source
    ../../src/ir/ir.cc
    ../../src/ast/ast.cc
    ../../src/lib/json/jsoncpp.cc
    ../../src/util/json.cc
    main.cc
)

include_directories(${PROJECT_SOURCE_DIR}/../../src)
include_directories(${PROJECT_SOURCE_DIR}/../../src/ir)
include_directories(${PROJECT_SOURCE_DIR}/../../src/lib)
include_directories(${PROJECT_SOURCE_DIR}/../../src/util)
include_directories(${PROJECT_SOURCE_DIR}/../../src/ast)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_executable(
    ${target}
    ${source}
)

# llvm lib
find_package(
    LLVM
    REQUIRED
    CONFIG
)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(
    ${target}
    ${llvm_libs}
)
