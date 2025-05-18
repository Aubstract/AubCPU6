cmake_minimum_required(VERSION 3.29)
project(AubCPU6)

set(CMAKE_CXX_STANDARD 20)

add_executable(AubCPU6
        src/assembler/main.cpp
        src/assembler/opcodes.hpp
        src/assembler/error.cpp
        src/assembler/error.cpp
        src/assembler/error.h
        src/assembler/Line.h
        src/assembler/Label.h
        src/assembler/Macro.h
        src/assembler/Constant.h
)
