//
// Created by Aubrey on 5/18/2025.
//

#ifndef AUBCPU6_FILE_IO_HPP
#define AUBCPU6_FILE_IO_HPP

#include "../preprocess/Line.hpp"
#include "../utils/error.hpp"

#include <filesystem>
#include <fstream>
#include <vector>

void read_file_into_lines(std::filesystem::path& path, std::vector<Line>& lines);

#endif //AUBCPU6_FILE_IO_HPP
