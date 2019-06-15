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
const int PURPLE = 35;
const int GREY = 36;
const int WHITE = 37;

std::string setColor(const std::string &str, int color);
std::string setBackground(const std::string &str, int color);
void pretty_print(const std::string type, const std::string msg, std::pair<int, int> left, std::pair<int, int> right);
} // namespace pretty