//
// Created by Aubrey on 5/18/2025.
//

#ifndef AUBCPU6_PREPROCESS_HPP
#define AUBCPU6_PREPROCESS_HPP

#include "../common/ASTNode.hpp"
#include "../front_end/file_io.hpp"
#include "../common/SourceLine.hpp"
#include "../utils/string_operations.hpp"
#include "../common/Token.hpp"

#include <vector>

void sanitize(std::vector<SourceLine>& src_lines)
{
    // For loop to iterate through the lines starting from the last line
    // and remove comments, leading/trailing whitespace, and empty lines
    for (int i = src_lines.size() - 1; i >= 0; --i)
    {
        std::string& line = src_lines[i].line;

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
            src_lines.erase(src_lines.begin() + i);
        }
    }
}



void construct_AST(std::vector<SourceLine>& src_lines,
                   ASTNode* root,
                   std::vector<ASTNode*>& macros,
                   std::vector<ASTNode*>& constants,
                   std::vector<ASTNode*>& labels)
{
    // Construct the AST from the sanitized source lines
    for (int i=0; i < src_lines.size(); ++i)
    {
        std::vector<std::string> tokens = split(src_lines[i].line, " ");

        for (int j=0; j < tokens.size(); ++j)
        {
            if (tokens[j] == "#include") // Include statement case
            {
                if (tokens.size() != 2)
                {
                    quit_with_error(src_lines[i], "Invalid number of tokens in include statement");
                }

                root->children.push_back(new ASTNode(INCLUDE_STATEMENT, tokens[j]));
                root->children.back()->children.push_back(new ASTNode(INCLUDE_PATH, tokens[j+1]));
            }
            else if (tokens[j] == "MACRO") // Macro def case
            {
                if (tokens.size() < 2)
                {
                    quit_with_error(src_lines[i], "Invalid number of tokens in macro definition");
                }

                j++; // Increment to macro name index
                macros.push_back(new ASTNode(MACRO_DEF, tokens[j]));

                j++; // Increment to first macro arg

                // Add the macro arguments to the macro node
                while (j < tokens.size())
                {
                    macros.back()->children.push_back(new ASTNode(MACRO_ARG, tokens[j]));
                }

                // Add the body to the macro
                macros.back()->children.push_back(new ASTNode(MACRO_BODY));

                // Increment outer loop until end of macro
                while (split(src_lines[i].line, " ")[0] != "ENDMACRO")
                {
                    // Add the
                    ++i;
                }
            }
            else if (tokens[j].starts_with(".")) // Jump label case
            {

            }
            else if (tokens[j].ends_with(":")) // Constant label case
            {

            }
        }
    }
}

void preprocess(std::vector<SourceLine>& src_lines,
                ASTNode* root,
                std::vector<ASTNode*>& macros,
                std::vector<ASTNode*>& constants,
                std::vector<ASTNode*>& labels)
{
    // Sanitize the source lines
    sanitize(src_lines);

    // Tokenize
    std::vector<std::vector<Token>> tokens;
    for (const auto& line : src_lines)
    {
        std::vector<std::string> str_tokens = split(line.line, " ");
        for (size_t i = 0; i < str_tokens.size(); i++)
        {
            tokens.back().emplace_back(Token(str_tokens[i], line.src_file, line.line_num, i));
        }
    }

    // Extract macro definitions

    // Extract constant definitions

    // Extract jump label definitions

    // Construct the AST from the sanitized source lines
    construct_AST(src_lines, root, macros, constants, labels);
}

#endif //AUBCPU6_PREPROCESS_HPP
