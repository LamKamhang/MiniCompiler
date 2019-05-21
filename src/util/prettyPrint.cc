#include <iostream>
#include <string>
#include <sstream>
#include "prettyPrint.h"

std::string pretty::setColor(const std::string &str, int color)
{
    std::stringstream res;
    res << "\033[" << color << 'm' << str << "\033[0m";
    return res.str();
}
std::string pretty::setBackground(const std::string &str, int color)
{
    std::stringstream res;
    res << "\033[" << 10 + color << 'm' << str << "\033[0m";
    return res.str();
}