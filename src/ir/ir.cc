#include "block.h"
#include <exception>
#include "generator.h"
#include "ir.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <memory>
#include <stdlib.h>
#include "type.h"
#include "value.h"

// [Globals]
std::unique_ptr<llvm::LLVMContext> context;
std::unique_ptr<llvm::IRBuilder<>> builder;
std::unique_ptr<llvm::Module> module;
ir::Generator generator;
llvm::Function *fp = nullptr;

// [Generator]
llvm::Value *ir::Generator::LogError(const char *str)
{
    std::cout << str << std::endl;
    return nullptr;
}

void ir::Generator::init()
{
    auto &table = this->table;

    table.insert(std::pair<std::string, std::function<llvm::Value *(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "translation_unit",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> llvm::Value * {
            for (auto child : node->children)
            {
                if (!table.at(child->type)(child, block))
                {
                    return nullptr;
                }
            }
            return (llvm::Value *)1;
        }));
    table.insert(std::pair<std::string, std::function<llvm::Value *(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "statement_list",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> llvm::Value * {
            for (auto stat : node->children)
            {
                if (!table.at(stat->type)(stat, block))
                {
                    return nullptr;
                }
            }
            return (llvm::Value *)1;
        }));
    table.insert(
        std::pair<std::string, std::function<llvm::Value *(std::shared_ptr<ast::Node>, ir::Block &)>>(
            "compound_statement",
            [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> llvm::Value * {
                auto lastBlock = builder->GetInsertBlock();
                builder->SetInsertPoint(block.basic_block);
                for (auto stat : node->children)
                {
                    if (!table.at(stat->type)(stat, block))
                    {
                        return nullptr;
                    }
                }
                builder->SetInsertPoint(lastBlock);
                return (llvm::Value *)1;
            }));
    table.insert(std::pair<std::string, std::function<llvm::Value *(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "function_definition",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> llvm::Value * {
            // function return type
            auto func_decl = node;
            auto type_spec = func_decl->children[0]->getNameChild("type_specifier")->value;
            auto ret_type = block.getCustomType(type_spec);

            //  function name
            auto decl = func_decl->children[1];
            const std::string &fun_name = decl->children[0]->getNameChild("identifier")->value;

            // parameter list
            auto para_list = decl->children[1];
            std::vector<llvm::Type *> para_type;
            std::vector<std::string> para_name;
            for (auto para_decl : para_list->children)
            {
                //   parameter id
                const std::string &para_id =
                    para_decl->getNameChild("identifier")->value;
                para_name.push_back(para_id);

                // parameter type
                auto type = (llvm::Type *)table.at("declaration_specifiers")(para_decl, block);
                if (!type)
                {
                    return nullptr;
                }
                para_type.push_back(std::move(type));
            }

            // create function prototype
            llvm::FunctionType *function_type =
                llvm::FunctionType::get(ret_type, para_type, false);
            llvm::Function *function = llvm::Function::Create(
                function_type, llvm::GlobalValue::ExternalLinkage, fun_name,
                module.get());
            // set parameter name
            unsigned idx = 0;
            for (auto &arg : function->args())
            {
                arg.setName(para_name[idx++]);
            }
            if (!function)
            {
                return generator.LogError("fail to generate function.");
            }
            if (!function->empty())
            {
                return generator.LogError("function can not be redefined.");
            }

            // compound statements
            auto comp_stat = func_decl->children[2];
            ir::Block comp_block(&block);
            // use a new basic block
            comp_block.basic_block =
                llvm::BasicBlock::Create(*context, fun_name + "_block", function);
            // record function parameters
            for (auto &arg : function->args())
            {
                comp_block.SymbolTable[arg.getName()] = &arg;
            }
            // parse statements
            auto tmp = fp;
            fp = function;
            auto statments = table.at(comp_stat->type)(comp_stat, comp_block);
            if (!statments)
            {
                return generator.LogError("fail to generate statements block.");
            }
            fp = tmp;

            bool function_err = llvm::verifyFunction(*function);
            std::cout << "\n[generator] function verification result: "
                      << (function_err ? "wrong" : "correct") << std::endl;
            if (function_err)
            {
                module->print(llvm::errs(), nullptr);
            }
            return function;
        }));

    // declaration
    table.insert(std::pair<std::string, std::function<llvm::Value *(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "declaration_list",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> llvm::Value * {
            for (auto decl : node->children)
            {
                if (!table.at("declaration")(decl, block))
                {
                    return nullptr;
                }
            }
            return (llvm::Value *)1;
        }));
    table.insert(std::pair<std::string, std::function<llvm::Value *(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "declaration",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> llvm::Value * {
            auto decl_spec = node->children[0];
            // not implement 'const'/'static' yet
            auto root_type_str = decl_spec->getNameChild("type_specifier")->value;
            llvm::Type *decl_type = (llvm::Type *)table.at("declaration_specifiers")(decl_spec, block);
            if (!decl_type)
            {
                return nullptr;
            }

            auto init_decl_list = node->children[1];
            for (auto child : init_decl_list->children)
            {
                // not implement 'pointer' yet
                auto id_name = child->getNameChild("identifier")->value;
                llvm::Value *init_val;
                if (child->type == "init_declarator")
                {
                    auto init_list = child->children[1];
                    init_val = table.at("initializer_list")(init_list, block);
                }
                else if (child->type == "declarator")
                {
                    auto tmp = std::make_shared<ast::Node>(root_type_str, "0");
                    init_val = table.at("declarator")(tmp, block);
                }

                // if init_val not correct
                if (!init_val)
                {
                    return nullptr;
                }
                // if variable already exists, error
                if (!block.setSymbol(id_name, init_val))
                {
                    return generator.LogError("[ir-declaration] variable exits.");
                }
            }
            return (llvm::Value *)1;
        }));
    // table.insert(std::pair<std::string, std::function<llvm::Value *(std::shared_ptr<ast::Node>, ir::Block &)>>(
    //     "initializer_list",
    //     [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> llvm::Value * {
    //         auto &children = node->children;
    //         auto list_type = (llvm::ArrayType *)table.at("literal_list_type")(node, block);
    //         if (!list_type)
    //         {
    //             return nullptr;
    //         }
    //         llvm::AllocaInst *array_alloc = new llvm::AllocaInst(list_type,);
    //     }));
    // // [virtual] parse list type from literal list
    // table.insert(std::pair<std::string, std::function<llvm::Value *(std::shared_ptr<ast::Node>, ir::Block &)>>(
    //     "literal_list_type",
    //     [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> llvm::Value * {
    //         auto &children = node->children;
    //         // [not implement] multi type list
    //         auto first_child = children[0];
    //         llvm::Type *base_type = (llvm::Type *)table.at("declaration_specifiers")(first_child, block);
    //         if (!base_type)
    //         {
    //             return nullptr;
    //         }
    //         auto num = children.size();
    //         return (llvm::Value *)llvm::ArrayType::get(base_type, num);
    //     }));

    // jump node
    table.insert(std::pair<std::string, std::function<llvm::Value *(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "return_expr",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> llvm::Value * {
            auto expr = node->children[0];
            auto ret_val = table.at(expr->type)(expr, block);
            // not check yet
            return builder->CreateRet(ret_val);
        }));

    // basic type
    // [general] parse type for declaration_specifiers and parameter_declaration
    table.insert(std::pair<std::string, std::function<llvm::Value *(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "declaration_specifiers",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> llvm::Value * {
            // not implement const
            // not implement static
            // not implement array
            auto root_type = node->getNameChild("type_specifier")->value;
            auto ret_type = block.getCustomType(root_type);
            if (!ret_type)
            {
                return generator.LogError("[block] getting error type.");
            }
            return (llvm::Value *)ret_type;
        }));
    table.insert(std::pair<std::string, std::function<llvm::Value *(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "expression",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> llvm::Value * {
            auto child = node->children[0];
            return table.at(child->type)(child, block);
        }));
    table.insert(std::pair<std::string, std::function<llvm::Value *(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "int",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> llvm::Value * {
            auto val = atoi(node->value.c_str());
            return llvm::ConstantInt::get(*context, llvm::APInt(32, val, true));
        }));
    table.insert(std::pair<std::string, std::function<llvm::Value *(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "float",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> llvm::Value * {
            auto val = atof(node->value.c_str());
            return llvm::ConstantFP::get(*context, llvm::APFloat(val));
        }));
    table.insert(std::pair<std::string, std::function<llvm::Value *(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "char",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> llvm::Value * {
            auto val = atoi(node->value.c_str());
            return llvm::ConstantInt::get(*context, llvm::APInt(8, val, false));
        }));
}

llvm::Value *ir::Generator::generate(std::shared_ptr<ast::Node> &object)
{
    auto &table = this->table;
    try
    {
        // Main loop

        // Create infrastructure
        ir::createIrUnit();
        ir::Block global;
        auto &root = object;
        auto &type = root->type;
        if (type != "translation_unit")
        {
            throw "\n[ir] error: type is not translation_unit\n";
        }

        // Generate ir from a tree
        auto res = table.at(type)(root, global);
        if (!res)
        {
            res->print(llvm::errs()); // print error msg if has
            throw "\n[ir] error at parsing a unit.\n";
        }

        // Print ir
        bool broken_debug_info = true;
        std::string err_str;
        llvm::raw_string_ostream es(err_str);
        bool module_broken = llvm::verifyModule(*module, &es);
        es.flush();
        std::cout << "\n[main] module verification result: "
                  << (module_broken ? "wrong" : "correct") << std::endl;
        if (module_broken)
        {
            std::cout << "Error message:\n"
                      << err_str << std::endl;
        }
        else
        {
            std::string out_str;
            llvm::raw_string_ostream os(out_str);
            os << *module;
            os.flush();
            std::cout << "\n[main] Generated IR:\n" + out_str << std::endl;
        }
        return (llvm::Value *)1;
    }
    catch (char const *error)
    {
        // if an ast errors when generating IR
        std::cout << error;
        module->print(llvm::errs(), nullptr); // print error msg
    }
    return nullptr;
}

// [IR]
void ir::createIrUnit()
{
    context = llvm::make_unique<llvm::LLVMContext>();
    builder = llvm::make_unique<llvm::IRBuilder<>>(*context);
    module = llvm::make_unique<llvm::Module>("my JIT", *context);
}

// [Value]
void ir::CustomValue::setType(ir::Type *type)
{
    this->type = type;
    this->pos = this->type->typeStack.size();
}

// [Block]
llvm::Value *ir::Block::getSymbol(const std::string &name)
{
    Block *node = this;
    while (node)
    {
        auto val = node->SymbolTable.at(name);
        if (val)
        {
            return val;
        }
        node = node->parent;
    }
    return nullptr;
}
bool ir::Block::setSymbol(const std::string &name, llvm::Value *val)
{
    if (this->SymbolTable.at(name))
    {
        return false;
    }
    else
    {
        this->SymbolTable[name] = val;
        return true;
    }
}
llvm::Type *ir::Block::getCustomType(const std::string &type)
{
    if (type == "int")
    {
        return std::move(llvm::Type::getInt32Ty(*context));
    }
    if (type == "char")
    {
        return std::move(llvm::Type::getInt8Ty(*context));
    }
    return nullptr;
}