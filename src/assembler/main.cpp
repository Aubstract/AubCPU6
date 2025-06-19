// Created 2025-04-14

#include "utils/AST.hpp"
#include "assemble/assemble.hpp"
#include "utils/file_io.hpp"
#include "preprocess/preprocess.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

int main()
{
    // Load the source file
    std::string src_file_path_str;
    std::cout << "Enter the source file path: ";
    std::cin >> src_file_path_str;
    std::filesystem::path src_file_path(src_file_path_str);

    std::vector<StrLine> str_lines;
    read_file_into_str_lines(src_file_path, str_lines);

    // Preprocess the source file
    ASTNode root(Token{TOKEN_TYPE_SRC_FILE, ""});
    preprocess(root, str_lines, src_file_path);

    // Assemble the source file
    //assemble(lines);

    return 0;
}

// I wonder if it would make sense to have an implicit "cal main" at the top of every program
// kinda like how C++ calls main().
// It would mean that there would *have* to be a "main:" in every program.
// That would allow me to link other files to the top of the src file nicely.
// That would require that the std_lib_macros be linked *above* the implicit "cal main"
// since "cal" is a std_lib_macro.
//
// So a linked file would look like this:
// 1. std_lib_macros
// 2. "cal main"
// 3. linked_src_files
// 4. main_src_file
//        - link statements
//        - subroutines, macros, and labels
//        - main:
//        - code
//
// I also wonder if the std_lib should *only* be made of macros, since I don't want to include
// subroutines that are never used, and I currently have no way of detecting if a subroutine is
// or is not used.
//
// Assembler process:
// - Read in the source file
//      - Convert it to a vector of Lines (maybe this should happen after linking)
// - Link other files to it
//      - Recursively look for link statements and attach
//        the linked files to the top (?) of the src file
// - Sanitize the src file
//      - remove comments and whitespace
// - First debug pass
//      - ensure opcodes have the correct number of operands
// - Resolve labels and macros
//      - macros first, otherwise labels cant be resolved to literal addresses
// - Second debug pass
//      - ensure all code is now made of opcodes (no more macros)
//      - ensure no more labels exist
// - Assemble
//      - Results in a .bin