cmake_minimum_required(VERSION 3.29)
project(AubCPU6)

set(CMAKE_CXX_STANDARD 20)

add_executable(AubCPU6
        src/assembler/main.cpp
        src/assembler/assemble/Opcode.hpp
        src/assembler/utils/error.cpp
        src/assembler/utils/error.cpp
        src/assembler/utils/error.hpp
        src/assembler/preprocess/Line.hpp
        src/assembler/assemble/Label.hpp
        src/assembler/preprocess/Macro.hpp
        src/assembler/assemble/Constant.hpp
        src/assembler/utils/Token.hpp
        src/assembler/assemble/Instruction.hpp
        src/assembler/preprocess/preprocess.hpp
        src/assembler/utils/string_operations.cpp
        src/assembler/utils/string_operations.hpp
        src/assembler/utils/file_io.hpp
        src/assembler/preprocess/definitions.hpp
        src/assembler/assemble/assemble.hpp
        src/assembler/assemble/assemble_defs.hpp
        src/assembler/utils/AST.hpp
        src2/assembler/main.cpp
        src2/assembler/common/ASTNode.hpp
        src2/assembler/preprocess/preprocess.hpp
        src2/assembler/front_end/file_io.hpp
        src2/assembler/error/error.hpp
        src2/assembler/common/SourceLine.hpp
        src2/assembler/utils/string_operations.hpp
        src2/assembler/common/definitions.hpp
        src2/assembler/common/Token.hpp
)
