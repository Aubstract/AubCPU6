//
// Created by aub on 4/17/25.
//

#ifndef LINE_H
#define LINE_H

#include <string>

struct Line
{
    std::string src_file;
    std::string line;
    int line_num;

    Line(const std::string& _src_file, const int _line_num, const std::string& _line)
        : src_file(_src_file), line(_line), line_num(_line_num) {}
};

#endif //LINE_H
