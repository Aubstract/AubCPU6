//
// Created by Aubrey on 5/18/2025.
//

#ifndef AUBCPU6_ASTNODE_HPP
#define AUBCPU6_ASTNODE_HPP

#include "Token.hpp"

#include <string>
#include <vector>

struct ASTNode
{
    Token token;
    std::vector<ASTNode*> children;
};

#endif //AUBCPU6_ASTNODE_HPP
