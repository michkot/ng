cmake_minimum_required(VERSION 2.8)
project(ng)

# for ValuesZ3 Z3prover
set(Z3 0)

# packages
find_package(LLVM 3.9 REQUIRED CONFIG)
message(STATUS "LLVM version ${LLVM_PACKAGE_VERSION}")
find_package(Boost 1.62 REQUIRED)
find_package(GTest)
if(Z3)
    # find_package(Z3)
endif()

# compiler directives

set(USE_SANITIZERS OFF)
set(CMAKE_VERBOSE_MAKEFILE off)

# set(GLOBALFLAGS -fPIC)
if (USE_SANITIZERS AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(SANIT_FLAGS "-fsanitize=address -fsanitize=leak -fsanitize=undefined")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
else()
    set(SANIT_FLAGS "")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -o3")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${SANIT_FLAGS} -g")

#file(GLOB SOURCE_FILES
#    *.hh
#    *.cc
#)

set(SOURCE_FILES
    src/Definitions.hh
    src/DummyOperations.hh
    src/DummyState.hh
    src/enum_flags.h
    src/Exceptions.cc
    src/Exceptions.hh
    src/ForwardNullAnalysis.hh
    src/FrontedValueMapper.hh
    src/FrontendLlvm.cc
    src/FrontendLlvm.hh
    src/General.hh
    src/ICfgNode.hh
    src/IOperation.hh
    src/IState.hh
    src/LlvmGlobals.cc
    src/LlvmGlobals.hh
    src/main.cpp
    src/OperationArgs.hh
    src/StateStorage.cc
    src/StateStorage.hh
    src/Type.cc
    src/Type.hh
    src/ValueContainerV1.cc
    src/ValueContainerV1.hh
    src/Values.cc
    src/Values.hh
)

if(Z3)
    add_definitions(-DUSE_Z3)
    set(SOURCE_FILES
        ${SOURCE_FILES}
        src/ValuesZ3.hh
        src/ValuesZ3.cc
    )
endif()

# targets

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
# dirty dirty dirty way -> assumes BINARY DIR is something like SRC_DIR/build etc.
add_custom_target(tests DEPENDS ${PROJECT_NAME} WORKING_DIRECTORY "../" COMMAND "bash" "./compile-inputs-to-ll.sh" COMMAND "${PROJECT_BINARY_DIR}/ng")
# includes

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${Z3_INCLUDE_DIRS})

# libs linking

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(LLVM_LIBS core irreader)

target_link_libraries(${PROJECT_NAME} ${LLVM_LIBS} ${Z3_LIBS})

# end
