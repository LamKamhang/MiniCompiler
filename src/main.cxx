#include <memory>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <llvm/IR/Verifier.h>

#include "parser/parser.hpp"
#include "ast/ast.h"
#include "ir/index.h"
#include "tc/tc.h"
#include "lib/json/json.h"
#include "ir/ir.h"


extern FILE* yyin;
extern std::shared_ptr<ast::Node> root;
extern std::string cwd;

int main (int argc, char **argv)
{

	if (argc >= 2)
	{
		if ((yyin = fopen(argv[1], "r")) == NULL)
		{
			fprintf(stderr, "Cannot open file %s\n", argv[1]);
			return 1;
		}
	}

	yyparse();
	std::string 

	// const std::string &path = "/test/function_definition/";
    // // const std::string &full_name(argv[1]);
    // const std::string &full_name = "2.c";
    // const std::string &file_name = full_name.substr(0, full_name.size() - 2);
    // Recover AST from Json::Value
    std::vector<std::shared_ptr<ast::Node>> forest;
	forest.push_back(root);
    // Generate IR form AST
    auto res = generator.generate(forest);
    if (!res)
    {
        std::cerr << "\n[main] error when generate ir.\n";
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


	if (argc >= 2)
	{
		fclose(yyin);
	}

	return 0;
}
