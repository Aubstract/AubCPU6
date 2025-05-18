//
// Created by Aubrey on 5/18/2025.
//

#ifndef AUBCPU6_PREPROCESS_HPP
#define AUBCPU6_PREPROCESS_HPP

#include "definitions.hpp"
#include "Line.hpp"
#include "Macro.hpp"
#include "../utils/file_io.hpp"

#include <filesystem>
#include <vector>

void link_recursive(std::vector<Line>& src_file_lines, const std::filesystem::path& current_dir);
void link(std::vector<Line>& src_file_lines, const std::filesystem::path& src_file_path);
void sanitize(std::vector<Line>& lines);
void construct_macros(std::vector<Line>& lines, std::vector<Macro>& macros);
void expand_macros(std::vector<Line>& lines, std::vector<Macro>& macros);
void preprocess(std::vector<Line>& lines, const std::filesystem::path& src_file_path);

#endif //AUBCPU6_PREPROCESS_HPP
