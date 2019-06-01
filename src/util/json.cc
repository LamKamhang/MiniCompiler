#include "lib/json/json.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

Json::Reader reader;

Json::Value parseJson(const std::string &path)
{
    std::ifstream is;
    is.open(path, std::ios::binary);
    Json::Value res;
    reader.parse(is, res, false);
    return res;
};