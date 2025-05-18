//
// Created by Aubrey on 5/18/2025.
//

#ifndef AUBCPU6_TOKEN_HPP
#define AUBCPU6_TOKEN_HPP

#include <string>

enum TokenType
{
    TOKEN_TYPE_INVALID = 0,
    TOKEN_TYPE_LABEL,
    TOKEN_TYPE_CONSTANT,
    TOKEN_TYPE_OPCODE,
    TOKEN_TYPE_REGISTER,
    TOKEN_TYPE_IMMEDIATE
};

struct Token
{
    TokenType type;
    std::string value;
};

#endif //AUBCPU6_TOKEN_HPP
