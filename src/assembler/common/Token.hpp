//
// Created by Aubrey on 6/19/2025.
//

#ifndef AUBCPU6_TOKEN_HPP
#define AUBCPU6_TOKEN_HPP

#include <string>

struct Token
{
    std::string token;
    std::string src_file;
    int line_num;
    int token_index;
};


#endif //AUBCPU6_TOKEN_HPP
