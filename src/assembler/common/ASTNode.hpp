//
// Created by Aubrey on 5/18/2025.
//

#ifndef AUBCPU6_ASTNODE_HPP
#define AUBCPU6_ASTNODE_HPP

#include <string>
#include <vector>

enum NodeType
{
    PROGRAM = 0,
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
    MACRO_CALL,
    UNTOKENIZED_LINE
};

struct ASTNode
{
    NodeType type;
    std::string value;
    std::vector<ASTNode*> children;
};

#endif //AUBCPU6_ASTNODE_HPP
