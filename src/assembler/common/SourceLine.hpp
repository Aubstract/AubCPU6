//
// Created by Aubrey on 5/18/2025.
//

#ifndef AUBCPU6_SOURCELINE_HPP
#define AUBCPU6_SOURCELINE_HPP

#include <string>

struct SourceLine
{
    std::string src_file;
    std::string line;
    int line_num;

    /*
    SourceLine(const std::string& src_file, int line_num, const std::string& line)
            : src_file(src_file), line(line), line_num(line_num) {}
    */
};

#endif //AUBCPU6_SOURCELINE_HPP
