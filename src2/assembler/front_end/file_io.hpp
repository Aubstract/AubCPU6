//
// Created by Aubrey on 5/18/2025.
//

#ifndef AUBCPU6_FILE_IO_HPP
#define AUBCPU6_FILE_IO_HPP

#include "../error/error.hpp"
#include "../common/SourceLine.hpp"

#include <filesystem>
#include <fstream>
#include <vector>

void read_file_into_src_lines(std::filesystem::path& path, std::vector<SourceLine>& lines)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        quit_with_error("Invalid file path: " + path.string());
    }

    int line_num = 1;
    std::string file_name = path.filename().string();
    std::string line;

    while (std::getline(file, line))
    {
        lines.emplace_back(file_name, line_num, line);
        line_num++;
    }

    file.close();
}

#endif //AUBCPU6_FILE_IO_HPP
