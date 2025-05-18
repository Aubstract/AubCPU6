//
// Created by Aubrey on 5/18/2025.
//

#ifndef AUBCPU6_STRING_OPERATIONS_HPP
#define AUBCPU6_STRING_OPERATIONS_HPP

#include <string>
#include <vector>

std::vector<std::string> split(const std::string& str, const std::string& delimiters);
std::string join(const std::vector<std::string>& tokens, const std::string& delimiter);

#endif //AUBCPU6_STRING_OPERATIONS_HPP
