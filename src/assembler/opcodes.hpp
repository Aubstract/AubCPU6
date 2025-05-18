// Created on 2025-04-14

#ifndef AUBCPU6_OPCODES_HPP
#define AUBCPU6_OPCODES_HPP

#include <string>

struct Opcode
{
    std::string mnemonic;
    int opcode;
    int num_operands;
};

// Opcode table for the AubCPU6
Opcode opcode_table[] = {
    {"hlt", 0, 0},
    {"jin", 1, 1},
    {"lod", 2, 2},
    {"ldi", 3, 2},
    {"sto", 4, 2},
    {"add", 5, 3},
    {"adi", 6, 2},
    {"sub", 7, 3},
    {"and", 8, 3},
    {"or", 9, 3},
    {"xor", 10, 3},
    {"rsh", 11, 3},
    {"in", 12, 2},
    {"out", 13, 2}
};


#endif //AUBCPU6_OPCODES_HPP
