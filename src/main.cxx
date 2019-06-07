#include <memory>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <llvm/IR/Verifier.h>
#include "lib/json/json.h"

#include "parser/parser.hpp"
#include "ast/ast.h"
#include "ir/ir.h"
#include "ir/index.h"
#include "tc/tc.h"

#define IN_C		(1<<0)
#define IN_JSON		(1<<1)
#define OUT_JSON	(1<<2)
#define OUT_OBJ		(1<<3)

using namespace std;

extern FILE* yyin;
extern shared_ptr<ast::Node> root;

int main (int argc, char **argv)
{
	vector<string> source_files;
	unsigned options = IN_C | OUT_JSON;

	for (int i = 1; i < argc; ++i)
	{
		string term(argv[i]);
		if (term.length() < 2)
		{
			exit(1);
			cerr << "unknown options" << endl;
		}
		if (term.at(0) == '-')
		{
			// todo
		}
		else
		{
			source_files.emplace_back(term);
		}
	}

	Json::StyledStreamWriter writer(" ");

	// c to obj
	if ((options & IN_C) && (options & OUT_OBJ))
	{
		for (auto & path_file : source_files)
		{
			string path, file;
			auto slash_pos = file.find_last_of('/');
			auto dot_pos = file.find_last_of('.');
			if (slash_pos == string::npos) 
			{
				path = "./";
				file = path_file.substr(0, dot_pos);
			}
			else
			{
				path = path_file.substr(0, slash_pos + 1);
				file = path_file.substr(slash_pos+1, dot_pos - slash_pos - 1);
			}
			yyin = fopen(path_file.c_str(), "r");
			if (yyin == NULL)
			{
				cerr << "Cannot open file" << file << endl;
				continue;
			}
			yyparse();
			fclose(yyin);
			if (options & OUT_JSON)
			{
				ofstream fout(path_file+".json");
				writer.write(fout, ast::exports(root));
				fout.close();
			}

			// Generate IR form AST
			auto res = generator.generate(root);
			if (!res)
			{
				cerr << "\n[main] error when generate ir.\n";
				return 0;
			}
			// Save IR to file
			string ir_code;
			llvm::raw_string_ostream ros(ir_code);
    		ros << *module;
			ofstream ir_file(path + file + ".ll");
			ir_file << ir_code;
    		ir_file.close();

			// Save target code to file
			ofstream tc_file(path + file + ".o");
			if (!tc::targetGenerate(tc_file))
			{
				cout << "\n[main] error when generate target code.\n";
				return 0;
			}
			tc_file.close();
		}
	}

	return 0;
}
