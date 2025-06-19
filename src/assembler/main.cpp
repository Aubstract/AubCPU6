//
// Created by Aubrey on 5/18/2025.
//

#include "front_end/file_io.hpp"
#include "preprocess/preprocess.hpp"

#include <iostream>
#include <filesystem>
#include <string>

int main()
{
    // Load the source file
    std::string src_file_path_str;
    std::cout << "Enter the source file path: ";
    std::cin >> src_file_path_str;
    std::filesystem::path src_file_path(src_file_path_str);

    // Read the file into a vector of SourceLine objects
    std::vector<SourceLine> src_file_lines;
    read_file_into_src_lines(src_file_path, src_file_lines);

    // Preprocess the source file
    auto* root = new ASTNode(PROGRAM, "program");
    std::vector<ASTNode*> macros;
    std::vector<ASTNode*> constants;
    std::vector<ASTNode*> labels;

    preprocess(src_file_lines, root, macros, constants, labels);

    // Assemble the source file


    return 0;
}