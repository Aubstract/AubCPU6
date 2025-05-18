//
// Created by aub on 4/25/25.
//

#ifndef MACRO_H
#define MACRO_H

#include "Line.hpp"

#include <string>
#include <vector>

struct MacroArg {
    std::string name;
    std::string value;
};

struct Macro {
    std::string name;
    std::vector<std::string> args;
    std::vector<Line> code;
    int times_used = 0;
};



#endif //MACRO_H
