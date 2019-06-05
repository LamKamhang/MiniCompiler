#include "lib/json/json.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <stdio.h>
#include "boost/filesystem.hpp"
Json::Reader reader;
auto cwd = boost::filesystem::current_path();
Json::Value Json::parseJson(const std::string &path)
{
    auto abs_path = cwd.string() + "/" + path;
    std::cout << "\n[json] cwd: " + cwd.string() << std::endl;
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