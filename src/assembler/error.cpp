// Created on 2025-04-17

#include "error.h"

void error(std::string message)
{
    std::cout << "################################################" << std::endl;
    std::cout << "Error: " << std::endl;
    std::cout << message << std::endl;
    std::cout << "################################################" << std::endl;
    std::exit(EXIT_FAILURE);
}

void error(Line line, std::string message)
{
    std::cout << "################################################" << std::endl;
    std::cout << "ERROR!" << std::endl;
    std::cout << message << std::endl;
    std::cout << line.src_file << ":" << line.line_num << ": " << line.line << std::endl;
    std::cout << "################################################" << std::endl;
    std::exit(EXIT_FAILURE);
}