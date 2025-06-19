//
// Created by Aubrey on 5/18/2025.
//

#ifndef AUBCPU6_ERROR_HPP
#define AUBCPU6_ERROR_HPP

#include "../common/SourceLine.hpp"

#include <iostream>
#include <string>

void quit_with_error(std::string message)
{
    std::cout << "################################################" << std::endl;
    std::cout << "Error: " << std::endl;
    std::cout << message << std::endl;
    std::cout << "################################################" << std::endl;
    std::exit(EXIT_FAILURE);
}

void quit_with_error(SourceLine line, std::string message)
{
    std::cout << "################################################" << std::endl;
    std::cout << "ERROR!" << std::endl;
    std::cout << message << std::endl;
    std::cout << line.src_file << ":" << line.line_num << ": " << line.line << std::endl;
    std::cout << "################################################" << std::endl;
    std::exit(EXIT_FAILURE);
}

#endif //AUBCPU6_ERROR_HPP
