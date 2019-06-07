#include "../../src/ir/index.h"
#include "../../src/tc/tc.h"
#include <ast/ast.h>
#include <fstream>
#include <iostream>
#include <lib/json/json.h>
#include <llvm/IR/Verifier.h>
#include <memory>
#include <string>
#include <vector>

extern std::string cwd;

int main(int argc, char **argv)
{
    // if (argc == 1)
    // {
    //     std::cout << "too few arguments" << std::endl;
    //     return 0;
    // }

    const std::string &path = "/test/function_definition/";
    // const std::string &full_name(argv[1]);
    const std::string &full_name = "2.c";
    const std::string &file_name = full_name.substr(0, full_name.size() - 2);
    // Read json
    auto root_json = Json::parseJson(path + file_name + ".json");
    if (root_json.empty())
    {
        std::cout << "\n[main] can't parse ast from json.\n";
        return 0;
    }
    // Recover AST from Json::Value
    std::shared_ptr<ast::Node> root;
    root = ast::imports(root_json);
    // Generate IR form AST
    auto res = generator.generate(root);
    if (!res)
    {
        std::cout << "\n[main] error when generate ir.\n";
        return 0;
    }
    // Save IR to file
    std::string ir_code;
    llvm::raw_string_ostream ros(ir_code);
    ros << *module;
    std::ofstream ir_file(cwd + path + file_name + ".ll");
    ir_file << ir_code;
    ir_file.close();
    // Save target code to file
    std::ofstream tc_file(cwd + path + file_name + ".o");
    if (!tc::targetGenerate(tc_file))
    {
        std::cout << "\n[main] error when generate target code.\n";
        return 0;
    }
    tc_file.close();
    return 1;
}