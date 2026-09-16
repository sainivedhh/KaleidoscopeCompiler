cmake_minimum_required(VERSION 3.10)
project(Kaleidoscope CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Main executable (for later, currently a placeholder)
# add_executable(kaleidoscope src/main.cpp)

# Compiler Library
add_library(CompilerLib src/Lexer.cpp src/Parser.cpp src/CodeGen.cpp)
target_include_directories(CompilerLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
llvm_map_components_to_libnames(llvm_libs Core Support Passes Target)
target_link_libraries(CompilerLib PUBLIC ${llvm_libs})

# Tests
enable_testing()
add_executable(LexerTest test/LexerTest.cpp)
target_link_libraries(LexerTest CompilerLib)
add_test(NAME LexerTest COMMAND LexerTest)

add_executable(ParserTest test/ParserTest.cpp)
target_link_libraries(ParserTest CompilerLib)
add_test(NAME ParserTest COMMAND ParserTest)

add_executable(CodeGenTest test/CodeGenTest.cpp)
target_link_libraries(CodeGenTest CompilerLib)
add_test(NAME CodeGenTest COMMAND CodeGenTest)
