//
// Created by Aubrey on 5/18/2025.
//

#ifndef AUBCPU6_TOKEN_HPP
#define AUBCPU6_TOKEN_HPP

#include <string>

enum TokenType
{
    // Source file tokens
    TOKEN_TYPE_SRC_FILE,
    // Preprocessor tokens
    TOKEN_TYPE_INCLUDE_STATEMENT,
    TOKEN_TYPE_INCLUDE_PATH,
    // Macro tokens
    TOKEN_TYPE_MACRO_LABEL,
    TOKEN_TYPE_MACRO_ARG,
    TOKEN_TYPE_MACRO_END,
    // Assembly tokens
    TOKEN_TYPE_JUMP_LABEL,
    TOKEN_TYPE_CONSTANT_LABEL,
    TOKEN_TYPE_OPCODE,
    TOKEN_TYPE_REGISTER,
    TOKEN_TYPE_IMMEDIATE,
    TOKEN_TYPE_LABEL_USE
};

struct Token
{
    TokenType type;
    std::string lexeme; // Actual text that the Token represents
};

#endif //AUBCPU6_TOKEN_HPP
