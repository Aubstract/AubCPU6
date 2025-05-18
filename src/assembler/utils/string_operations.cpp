//
// Created by Aubrey on 5/18/2025.
//

#include "string_operations.hpp"

std::vector<std::string> split(const std::string& str, const std::string& delimiters)
{
    std::string str_copy = str;
    size_t pos = 0;
    std::string token;
    std::vector<std::string> tokens;

    while ((pos = str_copy.find_first_of(delimiters)) != std::string::npos)
    {
        token = str_copy.substr(0, pos);
        tokens.push_back(token);
        str_copy.erase(0, pos + 1);
    }
    tokens.push_back(str_copy);

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