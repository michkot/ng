cmake_minimum_required(VERSION 3.4)
project(bordel1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

#file(GLOB SOURCE_FILES
#    *.hh
#    *.cc
#)

set(SOURCE_FILES
    src/Definitions.hh
    src/DummyOperations.cc
    src/DummyOperations.hh
    src/FrontendLlvm.cc
    src/FrontendLlvm.hh
    src/Exceptions.cc
    src/Exceptions.hh
    src/DummyState.hh
    src/General.hh
    src/ICfgNode.hh
    src/IOperation.hh
    src/IState.hh
    src/main.cpp
    src/StateStorage.hh
    src/StateStorage.cc
    src/Type.hh
    src/Values.hh)

add_executable(bordel1 ${SOURCE_FILES})

# Find the libraries that correspond to the LLVM components
# that we wish to use
find_package(LLVM REQUIRED CONFIG)
llvm_map_components_to_libnames(LLVM_LIBS core irreader)

target_link_libraries(bordel1 ${LLVM_LIBS})