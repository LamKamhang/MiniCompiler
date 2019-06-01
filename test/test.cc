
#include <gtest/gtest.h>
// #include "../include/json/jsoncpp.cc"
#include <fstream>
#include <string>
#include <iostream>

TEST(testJson, test0)
{
    // Json::Reader reader;
    // Json::Value root;
    // const std::string fileName = "a.json";
    // std::ifstream is;
    // is.open(fileName, std::ios::binary);
    // if (reader.parse(is, root, false))
    // {
    //     std::cout << root["type"].asString() << std::endl;
    //     std::cout << root["value"].asDouble() << std::endl;
    //     // for (auto i = 0; i < root.size(); ++i)
    //     // {
    //     //     std::cout << root[]
    //     //     // std::cout << root["child"][i]["type"].asString() + " " + root["child"][i]["value"].asString() << std::endl;
    //     // }
    //     for (auto i = root.begin(); i != root.end(); ++i)
    //     {
    //         std::cout << root.type() << std::endl;
    //     }
    // }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}