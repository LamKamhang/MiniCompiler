#include "../../src/ir/index.h"
#include <lib/json/json.h>
#include <ast/ast.h>
#include <llvm/IR/Verifier.h>
#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <fstream>

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
    ir::Generator generator;
    auto res = generator.generate(forest);
    // Save IR to file
    std::string ir_code;
    llvm::raw_string_ostream ros(ir_code);
    ros << *Module;
    std::ofstream file(cwd + path + file_name + ".ll");
    file << ir_code;
    file.close();
}