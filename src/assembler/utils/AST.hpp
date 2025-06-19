//
// Created by Aubrey on 5/18/2025.
//

#ifndef AUBCPU6_AST_HPP
#define AUBCPU6_AST_HPP

#include "Token.hpp"

#include <string>
#include <vector>

struct ASTNode
{
    Token token;
    std::vector<ASTNode*> children;

    ASTNode(Token token) : token(token) {}
};

#endif //AUBCPU6_AST_HPP
