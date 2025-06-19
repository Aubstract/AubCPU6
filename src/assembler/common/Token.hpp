//
// Created by Aubrey on 6/19/2025.
//

#ifndef AUBCPU6_TOKEN_HPP
#define AUBCPU6_TOKEN_HPP

#include <string>

enum TokenType
{
    TEMP = 0,
    PROGRAM,
    INCLUDE_STATEMENT,
    INCLUDE_PATH,
    INSTRUCTION,
    OPCODE,
    REGISTER,
    IMMEDIATE,
    JUMP_LABEL_DEF,
    CONSTANT_LABEL_DEF,
    LABEL_USE,
    MACRO_DEF,
    MACRO_ARG,
    MACRO_BODY,
    MACRO_CALL
};

struct Token
{
    std::string token;
    TokenType type;
    std::string src_file;
    int line_num;
    int token_index;
};


#endif //AUBCPU6_TOKEN_HPP
