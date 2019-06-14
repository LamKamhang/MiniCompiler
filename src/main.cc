#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "lib/json/json.h"
#include <llvm/IR/Verifier.h>

#include "ast/ast.h"
#include "ir/index.h"
#include "ir/ir.h"
#include "parser/parser.hh"
#include "tc/tc.h"

#define IN_C (1 << 0)
#define IN_JSON (1 << 1)
#define OUT_JSON (1 << 2)
#define OUT_IR (1 << 3)
#define OUT_OBJ (1 << 4)

using namespace std;

extern FILE *yyin;
extern shared_ptr<ast::Node> root;
extern bool no_parse_error;
extern bool parse_pass;

int main(int _argc, char **_argv)
{
    int _argc = argc;
    char **_argv = argv;
    // int _argc = 5;
    // string _argv[] = {"ncc", "test/function_definition/2.c", "-t=ir", "-t=json", "-t=obj"};

    vector<string> source_files;
    unsigned options = IN_C;

    for (int i = 1; i < _argc; ++i)
    {
        string term(_argv[i]);
        if (term.length() < 2)
        {
            cerr << "unknown options" << endl;
            exit(1);
        }
        if (term.at(0) == '-')
        {
            if (term.at(1) == 't' && term.at(2) == '=')
            {
                std::string des_type = term.substr(3, term.size());
                if (des_type == "json")
                {
                    options |= OUT_JSON;
                }
                else if (des_type == "obj")
                {
                    options |= OUT_OBJ;
                }
                else if (des_type == "ir")
                {
                    options |= OUT_IR;
                }
            }
        }
        else
        {
            source_files.emplace_back(term);
        }
    }

    Json::StyledStreamWriter writer(" ");

    // c to obj
    if ((options & IN_C))
    {
        for (auto &file : source_files)
        {
            string wo_ext = file.substr(0, file.find_last_of('.'));

            // Parse AST from C code
            yyin = fopen(file.c_str(), "r");
            if (yyin == NULL)
            {
                cerr << "Cannot open file" << file << endl;
                continue;
            }
            yyparse();
            fclose(yyin);
            if (!no_parse_error && parse_pass)
            {
                exit(1);
            }
            if (options & OUT_JSON)
            {
                ofstream ast_file(wo_ext + ".json");
                writer.write(ast_file, ast::exports(root));
                ast_file.close();
            }

            // Generate IR form AST
            auto res = generator.Generate(root);
            if (!res)
            {
                cerr << "\n[main] error when generate ir.\n";
                return 0;
            }
            // Save IR to file
            if (options & OUT_IR)
            {
                string ir_code;
                llvm::raw_string_ostream ros(ir_code);
                ros << *module;
                ros.flush();
                std::cout << "\n[main] Generated IR:\n" + ir_code << std::endl;
                ofstream ir_file(wo_ext + ".ll");
                ir_file << ir_code;
                ir_file.close();
            }

            // Generate target code from IR
            if (options & OUT_OBJ)
            {
                ofstream tc_file(wo_ext + ".o");
                if (!tc::targetGenerate(tc_file))
                {
                    cout << "\n[main] error when generate target code.\n";
                    return 0;
                }
                tc_file.flush();
                tc_file.close();
            }
        }
    }

    return 0;
}
