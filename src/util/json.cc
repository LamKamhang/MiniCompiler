#include "lib/json/json.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>

Json::Reader reader;
std::string cwd(getcwd(NULL, 0));

Json::Value Json::parseJson(const std::string &path)
{
    auto abs_path = cwd + "/" + path;
    std::cout << "\n[json] cwd: " + cwd << std::endl;
    std::ifstream is;
    is.open(abs_path, std::ios::binary);
    if (!is)
    {
        std::cout << "\n[json] can't open file" << std::endl;
    }
    Json::Value res;
    reader.parse(is, res, false);
    std::cout << "\n[json] successfully parse: " + abs_path << std::endl;
    return res;
};