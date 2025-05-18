// Created 2025-04-14
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Constant.h"
#include "error.h"
#include "Label.h"
#include "Line.h"
#include "Macro.h"

const std::filesystem::path STD_LIB_PATH("../src/assembler/std_lib/std_lib.txt");
const std::string START_MACRO_NAME = "start";


std::vector<std::string> split(std::string& str, const std::string& delimiters)
{
    size_t pos = 0;
    std::string token;
    std::vector<std::string> tokens;

    while ((pos = str.find_first_of(delimiters)) != std::string::npos)
    {
        token = str.substr(0, pos);
        tokens.push_back(token);
        str.erase(0, pos + 1);
    }
    tokens.push_back(str);

    return tokens;
}


std::string join(const std::vector<std::string>& tokens, const std::string& delimiter)
{
    std::string str;
    for (const auto& token : tokens)
    {
        if (!str.empty())
            str += delimiter;
        str += token;
    }

    return str;
}


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


void link_recursive(std::vector<Line>& src_file_lines, const std::filesystem::path& current_dir)
{
    // Link statements will be written in C++ style, for example:
    // #include "../file/path.txt"
    // #include <library_file>

    for (int i = 0; i < src_file_lines.size(); ++i)
    {
        std::string line = src_file_lines.at(i).line;

        if (line.starts_with('#') && line.find("include") != std::string::npos)
        {
            // Get relative path
            size_t start = line.find('"') + 1;
            size_t end = line.find('"', start);
            std::string relative_include_path = line.substr(start, end - start);

            // Resolve relative to the current file's directory
            std::filesystem::path resolved_include_path = current_dir / relative_include_path;
            std::vector<Line> linked_lines;

            read_file_into_lines(resolved_include_path, linked_lines);

            std::filesystem::path next_dir = resolved_include_path.parent_path();

            link_recursive(linked_lines, next_dir);

            src_file_lines.insert(src_file_lines.begin() + i, linked_lines.begin(), linked_lines.end());
            src_file_lines.erase(src_file_lines.begin() + i + linked_lines.size());
        }
    }
}


void link(std::vector<Line>& src_file_lines, const std::filesystem::path& src_file_path)
{
    // Link all files recursively
    link_recursive(src_file_lines, src_file_path.parent_path());

    // insert "start" macro at top
    src_file_lines.insert(src_file_lines.begin(), Line("linker", 0, START_MACRO_NAME));

    // insert std_lib at top
    std::vector<Line> std_lib_lines;
    std::filesystem::path std_lib_path(STD_LIB_PATH);
    read_file_into_lines(std_lib_path, std_lib_lines);
    link_recursive(std_lib_lines, std_lib_path.parent_path());
    src_file_lines.insert(src_file_lines.begin(), std_lib_lines.begin(), std_lib_lines.end());
}


void sanitize(std::vector<Line>& lines)
{
    // For loop to iterate through the lines starting from the last line
    // and remove comments, leading/trailing whitespace, and empty lines
    for (int i = lines.size() - 1; i >= 0; --i)
    {
        std::string& line = lines[i].line;

        // Remove comments that start with "//"
        size_t comment_pos = line.find("//");
        if (comment_pos != std::string::npos)
        {
            line.erase(comment_pos);
        }

        // Remove leading and trailing whitespace
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        // Remove all commas
        std::erase(line, ',');

        // Remove empty lines
        if (line.empty())
        {
            lines.erase(lines.begin() + i);
        }
    }
}


void construct_macros(std::vector<Line>& lines, std::vector<Macro>& macros)
{
    for (int i = 0; i < lines.size(); ++i)
    {
        std::string line = lines.at(i).line;

        if (line.starts_with("MACRO"))
        {
            // Get the macro name
            size_t start = line.find(' ') + 1;
            size_t end = line.find(' ', start);
            std::string macro_name = line.substr(start, end - start);

            // Get the arguments
            std::vector<std::string> args;
            while (end != std::string::npos)
            {
                start = end + 1;
                end = line.find(' ', start);
                args.push_back(line.substr(start, end - start));
            }

            // Find the end of the macro
            int end_macro_pos = 0;
            while (i + end_macro_pos < lines.size() && lines[i + end_macro_pos].line != "ENDM")
            {
                end_macro_pos++;
            }

            // Add the macro to the list of macros
            macros.emplace_back(macro_name, args, std::vector<Line>(lines.begin() + i + 1, lines.begin() + i + end_macro_pos));
        }
    }

    // Remove the macro definitions from the lines
    for (int i = lines.size() - 1; i >= 0; --i)
    {
        std::string line = lines.at(i).line;

        if (line.starts_with("MACRO"))
        {
            // Find the end of the macro
            int end_macro_pos = 0;
            while (i + end_macro_pos < lines.size() && lines[i + end_macro_pos].line != "ENDM")
            {
                end_macro_pos++;
            }

            // Remove the macro definition from the lines
            lines.erase(lines.begin() + i, lines.begin() + i + end_macro_pos + 1);
        }
    }
}

void expand_macros(std::vector<Line>& lines, std::vector<Macro>& macros)
{
    // Macros will be written like this:
    // MACRO name arg1 arg2
    //     code
    // ENDM

    construct_macros(lines, macros);

    // Expand the macros
    bool found_macro = false;
    for (int i = 0; i < lines.size(); found_macro ? i : i++)
    {
        std::string line = lines.at(i).line;
        found_macro = false;

        for (auto& macro : macros)
        {
            //if (split(line, " ").at(0) != macro.name)
            if (!line.starts_with(macro.name + " "))
            {
                continue;
            }

            found_macro = true;

            // Get the argument values
            std::vector<MacroArg> args;
            size_t start = line.find(' ') + 1;

            for (int j = 0; start != std::string::npos && j < macro.args.size(); ++j)
            {
                size_t end = line.find(' ', start);
                args.emplace_back(macro.args[j], line.substr(start, end - start));
                start = (end == std::string::npos) ? end : end + 1;
            }

            // Replace the macro call with the macro code
            lines.erase(lines.begin() + i);
            lines.insert(lines.begin() + i, macro.code.begin(), macro.code.end());

            // Replace macro arguments with their values in the lines
            for (int k = 0; k < macro.code.size(); ++k)
            {
                std::string& code_line = lines.at(i + k).line;

                for (const auto& arg : args)
                {
                    size_t arg_pos = code_line.find("[" + arg.name + "]");
                    if (arg_pos != std::string::npos)
                        code_line.replace(arg_pos, arg.name.length() + 2, arg.value);
                }
            }

            // Replace asterisks with the macro.times_used
            bool found_asterisk = false;
            for (int k = 0; k < macro.code.size(); ++k)
            {
                std::string& code_line = lines.at(i + k).line;

                size_t asterisk_pos = code_line.find('*');
                if (asterisk_pos != std::string::npos)
                {
                    found_asterisk = true;
                    code_line.replace(asterisk_pos, 1, std::to_string(macro.times_used));
                }
            }
            if (found_asterisk)
            {
                macro.times_used++;
            }
        }
    }
}

void resolve_constants(std::vector<Line>& lines, std::vector<Constant>& constants)
{
    // Constants will be written like this:
    // "constant_name: value"

    // Construct constants
    for (int i = 0; i < lines.size(); ++i)
    {
        std::string line = lines.at(i).line;

        if (line.find(':') != std::string::npos)
        {
            // Get the constant name and value
            size_t colon_pos = line.find(':');
            std::string constant_name = line.substr(0, colon_pos);
            int constant_value = std::stoi(line.substr(colon_pos + 1));

            // Add the constant to the list of constants
            constants.emplace_back(constant_name, constant_value);

            // Remove the constant definition from the lines
            lines.erase(lines.begin() + i);
        }
    }

    // Replace the constants with their values
    for (int i = 0; i < lines.size(); ++i)
    {
        std::string line = lines.at(i).line;

        for (const auto& constant : constants)
        {
            if (line.find(constant.name + " ") != std::string::npos)
            {
                // Replace the constant with its value
                line.replace(line.find(constant.name), constant.name.length(), std::to_string(constant.value));
            }
        }

        lines.at(i).line = line;
    }
}


void resolve_labels(std::vector<Line>& lines, std::vector<Label>& labels)
{
    // Labels will be written like this:
    // ".label_name"

    // Construct labels
    for (int i = 0; i < lines.size(); ++i)
    {
        if (lines.at(i).line.starts_with('.'))
        {
            std::string label_name = lines.at(i).line.substr(1);
            labels.emplace_back(label_name, i+1); // +1 to get the address of the next line
            lines.erase(lines.begin() + i);
        }
    }

    // Replace the labels with their addresses
    for (int i = 0; i < lines.size(); ++i)
    {
        std::string line = lines.at(i).line;

        for (const auto& label : labels)
        {
            if (line.find(label.name) != std::string::npos)
            {
                // Replace the label with its address
                line.replace(line.find(label.name), label.name.length(), std::to_string(label.address));
            }
        }

        lines.at(i).line = line;
    }
}


int main()
{
    std::string src_file_path_str;
    std::vector<Line> lines;
    std::vector<Macro> macros;
    std::vector<Constant> constants;
    std::vector<Label> labels;

    std::cout << "Enter the source file path: ";
    std::cin >> src_file_path_str;
    std::filesystem::path src_file_path(src_file_path_str);

    read_file_into_lines(src_file_path, lines);

    link(lines, src_file_path);

    sanitize(lines);

    expand_macros(lines, macros);

    // resolve constants
    resolve_constants(lines, constants);

    // resolve labels
    //resolve_labels(lines, labels);

    // resolve expressions

    // assemble

    // Just for testing, print the lines
    for (const auto& line : lines)
    {
        std::cout << line.src_file << ":" << line.line_num << ": " << line.line << std::endl;
    }

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