#pragma once

#include <string>

enum pdArgType {BANG, CHAR, FLOAT};

struct pdArg {
    int token;
    pdArgType type;
    std::string str;
    float flt;
    // just add msg name
    // arg c
    // arg v as t_atom*
};