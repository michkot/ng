cmake_minimum_required(VERSION 3.4)
project(bordel1)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "LLVM version ${LLVM_PACKAGE_VERSION}")
find_package(Boost REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

#file(GLOB SOURCE_FILES
#    *.hh
#    *.cc
#)

set(SOURCE_FILES
    src/Definitions.hh
    src/DummyOperations.cc
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
    #src/ValuesZ3.cc
    #src/ValuesZ3.hh
)

add_executable(bordel1 ${SOURCE_FILES})

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(LLVM_LIBS core irreader)

target_link_libraries(bordel1 ${LLVM_LIBS})
