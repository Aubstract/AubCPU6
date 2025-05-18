//
// Created by Aubrey on 5/18/2025.
//

#include "preprocess.hpp"

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


void preprocess(std::vector<Line>& lines, const std::filesystem::path& src_file_path)
{
    // Macros are expanded in the preprocessor, so they don't need to be saved beyond this point
    std::vector<Macro> macros;

    link(lines, src_file_path); // Recursively links files, inserts std_lib, and "start" macro
    sanitize(lines); // Removes comments, whitespace, and empty lines
    construct_macros(lines, macros); // Constructs macros and removes them from the src file
    expand_macros(lines, macros); // Expands macros in the src file
}