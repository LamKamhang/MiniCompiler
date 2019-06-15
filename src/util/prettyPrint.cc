#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "prettyPrint.h"

extern std::string current_file;

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

void pretty::pretty_print(const std::string type, const std::string msg, std::pair<int, int> left, std::pair<int, int> right)
{
    if (left.first > right.first || (left.first == right.first && left.second > right.second))
    {
        std::cerr << "[print_error internal error] inivalid left and right arguments." << std::endl;
        return;
    }

    std::ifstream fin(current_file);
    if (!fin.is_open())
    {
        std::cerr << "[print_error internal error] cannot open file " << current_file << "." << std::endl;
        return;
    }
    std::string line;

    int color;
    std::cerr << current_file << ":" << left.first << ":" << left.second << ": ";
    if (type == "Warning")
    {
        color = YELLOW;
        std::cerr << setColor("Warning: ", color);
    }
    else if (type == "Error")
    {
        color = RED;
        std::cerr << setColor("Error: ", color);
    }
    else
    {
        color = GREEN;
    }

    std::cerr << setColor(msg, color) << std::endl;
    if (left.first == right.first)
    {
        for (int i = 0; i < left.first; ++i)
        {
            getline(fin, line);
        }
        std::cerr << line << std::endl;

        for (int i = 0; i < left.second; ++i)
        {
            std::cerr << " ";
        }
        for (int i = left.second; i <= right.second; ++i)
        {
            std::cerr << setColor("~", PURPLE);
        }
        std::cerr << std::endl;
    }
    else if (left.first < right.first)
    {
        for (int i = 0; i < left.first; ++i)
        {
            getline(fin, line);
        }
        std::cerr << line << std::endl;
        for (int i = 0; i < left.second; ++i)
        {
            std::cerr << " ";
        }
        for (int i = left.second; i <= line.size(); ++i)
        {
            std::cerr << setColor("~", PURPLE);
        }
        std::cerr << std::endl;
        for (int i = left.first; i < right.first; ++i)
        {
            getline(fin, line);
            std::cerr << line << std::endl;
            for (int i = 0; i < line.size(); ++i)
            {
                std::cerr << setColor("~", PURPLE);
            }
            std::cerr << std::endl;
        }
    }
}