#pragma once
#include "include/json.h"
#include "include/json-forwards.h"
namespace Json
{
Json::Value parseJson(const std::string &path);
}