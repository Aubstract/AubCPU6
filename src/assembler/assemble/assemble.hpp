//
// Created by Aubrey on 5/18/2025.
//

#ifndef AUBCPU6_ASSEMBLE_HPP
#define AUBCPU6_ASSEMBLE_HPP

#include "assemble_defs.hpp"
#include "Constant.hpp"
#include "../utils/error.hpp"
#include "Instruction.hpp"
#include "Label.hpp"
#include "../preprocess/Line.hpp"
#include "../utils/string_operations.hpp"

#include <algorithm>
#include <cassert>
#include <vector>

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


void tokenize(const std::vector<Line>& lines, std::vector<Instruction>& instr)
{
    for (auto& line : lines)
    {
        std::vector<std::string> str_toks = split(line.line, " ");
        instr.emplace_back();

        for (auto& str_tok : str_toks)
        {
            TokenType type = TOKEN_TYPE_INVALID;

            if (str_tok.starts_with('.'))
            {
                type = TOKEN_TYPE_LABEL;
            }
            else if (str_tok.ends_with(':'))
            {
                type = TOKEN_TYPE_CONSTANT;
            }
            else if (std::find(opcode_mnemonics.begin(), opcode_mnemonics.end(), str_tok) != opcode_mnemonics.end())
            {
                type = TOKEN_TYPE_OPCODE;
            }
            else if (std::find(register_names.begin(), register_names.end(), str_tok) != register_names.end())
            {
                type = TOKEN_TYPE_REGISTER;
            }
            else if (std::all_of(str_tok.begin(), str_tok.end(), ::isdigit))
            {
                type = TOKEN_TYPE_IMMEDIATE;
            }

            // Create a new token
            Token token(type, str_tok);

            // Add the token to the instruction
            instr.back().tokens.push_back(token);
        }
    }
}


void check_syntax(const std::vector<Instruction>& instructions, const std::vector<Line>& lines)
{
#ifndef NDEBUG
   assert(instructions.size() == lines.size());
#endif

    for (int i = 0; i < instructions.size(); ++i)
    {
        const auto& instr = instructions[i];
        const auto& line = lines[i];

        for (auto& token : instr.tokens)
        {
            if (token.type == TOKEN_TYPE_INVALID)
            {
                error(line, "Invalid token: " + token.value);
            }
        }

        // Check for valid opcode
        if (instr.tokens[0].type != TOKEN_TYPE_OPCODE)
        {
            error(line, "First token must be an opcode");
        }

        // Check for valid register
        if (instr.tokens.size() > 1 && instr.tokens[1].type != TOKEN_TYPE_REGISTER)
        {
            error(line, "Second token must be a register");
        }
    }
}


void assemble(std::vector<Line> lines)
{
    // Tokenize the lines
    std::vector<Instruction> instructions;
    tokenize(lines, instructions);

    std::vector<Constant> constants;
    std::vector<Label> labels;

    // resolve constants
    //resolve_constants(lines, constants);

    // resolve labels
    //resolve_labels(lines, labels);

    // Check the syntax of the instructions
    //check_syntax(instructions, lines);

    // assemble
}

#endif //AUBCPU6_ASSEMBLE_HPP
