//
// Created by Aubrey on 5/18/2025.
//

#ifndef AUBCPU6_DEFINITIONS_HPP
#define AUBCPU6_DEFINITIONS_HPP

#include "ASTNode.hpp"

#include <string>
#include <vector>
#include <filesystem>

const std::filesystem::path STD_LIB_PATH("../src2/assembler/std_lib/std_lib.txt");
const std::string START_MACRO_NAME = "start";

std::vector<std::string> opcode_mnemonics = {
        "hlt",
        "jin",
        "lod",
        "ldi",
        "sto",
        "add",
        "adi",
        "sub",
        "and",
        "or",
        "xor",
        "rsh",
        "in",
        "out"
};

std::vector<std::string> register_names = {
        "x0",
        "x1",
        "x2",
        "x3",
        "x4",
        "x5",
        "x6",
        "x7",
        "x8",
        "x9",
        "x10",
        "x11",
        "x12",
        "x13",
        "x14",
        "x15",
        "hard_zero",
        "return",
        "arg_a",
        "arg_b",
        "macro",
        "stack",
        "flags",
        "pc"
};

#endif //AUBCPU6_DEFINITIONS_HPP
