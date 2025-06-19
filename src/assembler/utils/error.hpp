//
// Created by aub on 4/17/25.
//

#ifndef ERROR_H
#define ERROR_H

#include "../preprocess/Line.hpp"

#include <iostream>
#include <string>

void quit_with_error(std::string);
void quit_with_error(Line, std::string);

#endif //ERROR_H
