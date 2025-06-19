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

void resolve_constants(std::vector<Instruction>& instructions, std::vector<Constant>& constants)
{
    // Constants will be written like this:
    // "constant_name: value"

    // Construct constants
    bool constant_found = false;
    for (int i = 0; i < instructions.size(); constant_found ? i : i++)
    {
        constant_found = false;
        if (instructions.at(i).tokens[0].type == TOKEN_TYPE_CONSTANT_LABEL)
        {
            constant_found = true;
            std::string constant_name = instructions.at(i).tokens[0].value;
            std::string constant_value = instructions.at(i).tokens[1].value;

            // Check if the constant is already defined
            for (const auto& constant : constants)
            {
                if (constant.name == constant_name)
                {
                    quit_with_error("Multiple definitions of constant: " + constant_name);
                }
            }

            // Add the constant to the list of constants
            constants.emplace_back(constant_name, std::stoi(constant_value));
            instructions.erase(instructions.begin() + i);
        }
    }

    // Replace the constants with their values
    for (auto& instr : instructions)
    {
        for (auto& token : instr.tokens)
        {
            if (token.type != TOKEN_TYPE_LABEL_USE)
            {
                continue;
            }

            for (const auto& constant : constants)
            {
                if (token.value == constant.name)
                {
                    // Replace the constant label with its immediate value
                    token = Token(TOKEN_TYPE_IMMEDIATE, std::to_string(constant.value));
                }
            }
        }
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


void check_constants_syntax(const std::vector<Instruction>& instructions, const std::vector<Line>& lines)
{
    for (int i = 0; i < instructions.size(); ++i)
    {
        const auto& instr = instructions[i];
        const auto& line = lines[i];

        if (instr.tokens[0].type == TOKEN_TYPE_CONSTANT_LABEL)
        {
            // Check for the correct number of tokens
            if (instr.tokens.size() != 2)
            {
                quit_with_error(line, "Incorrect number of tokens for constant definition");
            }

            // Check for correct type of tokens
            if (instr.tokens[1].type != TOKEN_TYPE_IMMEDIATE)
            {
                quit_with_error(line, "Second token must be an immediate value");
            }
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
                quit_with_error(line, "Invalid token: " + token.value);
            }
        }

        // Check for valid opcode
        if (instr.tokens[0].type != TOKEN_TYPE_OPCODE)
        {
            quit_with_error(line, "First token must be an opcode");
        }

        // Check for valid register
        if (instr.tokens.size() > 1 && instr.tokens[1].type != TOKEN_TYPE_REGISTER)
        {
            quit_with_error(line, "Second token must be a register");
        }
    }
}


void assemble(std::vector<Line> lines)
{
    // Tokenize the lines
    //std::vector<Instruction> instructions;
    //tokenize(lines, instructions);

    std::vector<Constant> constants;
    std::vector<Label> labels;

    // resolve constants
    check_constants_syntax(instructions, lines);
    resolve_constants(instructions, constants);

    // print program:
    for (const auto& instr : instructions)
    {
        for (const auto& token : instr.tokens)
        {
            std::cout << token.value << " ";
        }
        std::cout << std::endl;
    }

    // resolve labels
    //resolve_labels(lines, labels);

    // Check the syntax of the instructions
    //check_syntax(instructions, lines);

    // assemble
}

#endif //AUBCPU6_ASSEMBLE_HPP
