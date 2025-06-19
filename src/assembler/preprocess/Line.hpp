//
// Created by aub on 4/17/25.
//

#ifndef LINE_H
#define LINE_H

#include "../utils/Token.hpp"

#include <string>
#include <vector>

struct StrLine
{
    std::string src_file;
    std::string line;
    int line_num;

    StrLine(const std::string& src_file, int line_num, const std::string& line)
        : src_file(src_file), line(line), line_num(line_num) {}
};


struct Line
{
    std::string src_file;
    std::vector<Token> tokens;
    int line_num;

    Line(const std::string& src_file, int line_num)
        : src_file(src_file), line_num(line_num) {}
};


#endif //LINE_H
