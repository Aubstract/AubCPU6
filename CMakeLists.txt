cmake_minimum_required(VERSION 3.29)
project(AubCPU6)

set(CMAKE_CXX_STANDARD 20)

add_executable(AubCPU6
        src/assembler/main.cpp
        src/assembler/common/ASTNode.hpp
        src/assembler/common/definitions.hpp
        src/assembler/common/Token.hpp
        src/assembler/common/SourceLine.hpp
        src/assembler/error/error.hpp
        src/assembler/front_end/file_io.hpp
        src/assembler/preprocess/preprocess.hpp
        src/assembler/utils/string_operations.hpp
)
