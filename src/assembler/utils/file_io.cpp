//
// Created by Aubrey on 5/18/2025.
//

#include "file_io.hpp"

void read_file_into_lines(std::filesystem::path& path, std::vector<Line>& lines)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        error("Invalid file path: " + path.string()); // will exit the program
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