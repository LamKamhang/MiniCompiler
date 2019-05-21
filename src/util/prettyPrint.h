#include <iostream>
#include <string>
#include <sstream>

namespace pretty
{
const int BLACK = 30;
const int RED = 31;
const int GREEN = 32;
const int YELLOW = 33;
const int BLUE = 34;
const int PURPULE = 35;
const int GREY = 36;
const int WHITE = 37;

std::string setColor(const std::string &str, int color);
std::string setBackground(const std::string &str, int color);
} // namespace pretty