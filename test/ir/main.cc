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

int main()
{
    const std::string &path = "test/function_definition/";
    const std::string &file_name = "2";
    // Read json
    auto root = Json::parseJson(path + file_name + ".json");
    // Recover AST from Json::Value
    std::vector<std::shared_ptr<ast::Node>> forest;
    forest.push_back(ast::imports(root));
    // Generate IR form AST
    auto res = generator.generate(forest);
    // Save IR to file
    std::string ir_code;
    llvm::raw_string_ostream ros(ir_code);
    ros << *module;
    std::ofstream ir_file(cwd + path + file_name + ".ll");
    ir_file << ir_code;
    ir_file.close();
    // Save target code to file
    std::ofstream tc_file(cwd + path + file_name + ".o");
    tc::targetGenerate(tc_file);
    tc_file.close();
}