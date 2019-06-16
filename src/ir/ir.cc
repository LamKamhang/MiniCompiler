#include "ir.h"
#include "block.h"
#include "generator.h"
#include "type/index.h"
#include <exception>
#include <iostream>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <memory>
#include <sstream>
#include <stdlib.h>

// [Globals]
std::unique_ptr<llvm::LLVMContext> context;
std::unique_ptr<llvm::IRBuilder<>> builder;
std::unique_ptr<llvm::Module> module;
ir::Generator generator;

// Assistance
std::string ScanType(llvm::Type *type)
{
    std::stringstream ss;
    if (type->isIntegerTy())
        ss << "integer ";
    if (type->isPointerTy())
        ss << "pointer ";
    if (type->isFloatTy())
        ss << "float ";
    if (type->isFloatingPointTy())
        ss << "floatingP";
    if (type->isFunctionTy())
        ss << "function ";
    if (type->isVoidTy())
        ss << "void ";
    if (type->isStructTy())
        ss << "struct ";
    if (type->isArrayTy())
        ss << "array ";
    if (type->isEmptyTy())
        ss << "empty ";
    if (type->isLabelTy())
        ss << "label ";
    if (type->isMetadataTy())
        ss << "meta ";
    return ss.str();
}
std::string ScanType(llvm::Value *value)
{
    ScanType(value->getType());
}
void LogType(llvm::Value *value)
{
    llvm::errs() << ScanType(value) << "\n";
}
void LogType(llvm::Type *type)
{
    llvm::errs() << ScanType(type) << "\n";
}
// [general] parse type for declaration_specifiers and parameter_declaration
ir::BaseType *ParseBaseType(std::shared_ptr<ast::Node> node, ir::Block &block)
{
    // [not implement] static
    // [not implement] array
    bool is_const = false;
    ir::BaseType *base_type = nullptr;
    for (auto child : node->children)
    {
        auto &type_name = child->type;
        auto &type_val = child->value;
        if (type_name == "type_qualifier")
        {
            if (type_val == "const")
                is_const = true;
        }
        else if (type_name == "type_specifier")
        {
            // [not implement] other type
            base_type = type_val == "int" ? ir::IntegerTy::Get(32, true, is_const) : nullptr;
            is_const = false;
        }
    }
    if (!base_type) {

        generator.Error("[ir\\decl] can't resolve base type.");
    }
    return base_type;
}

// [Generator]
bool ir::Generator::Error(const std::string &str)
{
    std::cout << str << std::endl;
    return false;
}

void ir::Generator::Init()
{
    auto &generate_code = this->generate_code;
    auto &resolve_symbol = this->resolve_symbol;

    // init code gen method
    generate_code.insert(std::pair<std::string, std::function<bool(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "translation_unit",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> bool {
            for (auto child : node->children)
            {
                if (!generate_code.at(child->type)(child, block))
                    return false;
            }
            return true;
        }));
    generate_code.insert(std::pair<std::string, std::function<bool(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "statement_list",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> bool {
            for (auto stat : node->children)
            {
                if (!generate_code.at(stat->type)(stat, block))
                    return false;
            }
            return true;
        }));
    generate_code.insert(std::pair<std::string, std::function<bool(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "compound_statement",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> bool {
            for (auto stat : node->children)
            {
                if (!generate_code.at(stat->type)(stat, block))
                    return false;
            }
            return true;
        }));
    generate_code.insert(std::pair<std::string, std::function<bool(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "function_definition",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> bool {
            // function return type
            auto func_decl = node;
            // [not implement] detail type parse
            auto type_spec = func_decl->children[0]->getNameChild("type_specifier")->value;
            auto ret_type = ir::Type::GetConstantType(type_spec);

            //  function name
            auto decl = func_decl->children[1];
            const std::string &fun_name = decl->children[0]->getNameChild("identifier")->value;

            // parameter list
            auto para_list = decl->children[1];
            std::vector<llvm::Type *> para_type;
            std::vector<ir::Type *> para_type_list;
            std::vector<std::string> para_name;
            for (auto para_decl : para_list->children)
            {
                //   parameter id
                // [not implement] pointer analysis
                const std::string &para_id = para_decl->getNameChild("identifier")->value;
                para_name.push_back(para_id);

                // parameter type
                auto base_type = ParseBaseType(para_decl, block);
                if (!base_type)
                {
                    return false;
                }

                std::vector<ir::RootType *> type_stack;
                type_stack.push_back(base_type);
                auto full_type = ir::Type::Get(type_stack);

                para_type_list.push_back(full_type);
                para_type.push_back(base_type->_ty);
                // para_type.push_back(llvm::Type::getInt32Ty(*context));
                // LogType(llvm::Type::getInt32Ty(*context));
            }

            // create function prototype
            llvm::FunctionType *function_type =
                llvm::FunctionType::get(ret_type->BaseTy()->_ty, para_type, false);
            // check if exists a same name but different type function, which should be error
            auto maybe_fun = module->getFunction(fun_name);
            if (maybe_fun)
            {
                if (maybe_fun->getFunctionType() != function_type)
                    return generator.Error("[ir\\fun-def] define a same name function but with different type.");
            }
            llvm::Function *function = llvm::Function::Create(
                function_type, llvm::GlobalValue::ExternalLinkage, fun_name,
                module.get());
            if (!function || !function_type)
                return generator.Error("[ir\\fun-def] can't create function.");
            // set parameter name
            unsigned idx = 0;
            for (auto &arg : function->args())
            {
                arg.setName(para_name[idx++]);
            }
            if (!function)
            {
                return generator.Error("[ir\\fun-def] fail to generate function.");
            }
            if (!function->empty())
            {
                return generator.Error("[ir\\fun-def] function can not be redefined.");
            }

            // compound statements
            auto comp_stat = func_decl->children[2];
            ir::Block comp_block(&block);

            // use a new basic block
            auto comp_bb = llvm::BasicBlock::Create(*context, fun_name + "_block", function);
            // record old block
            auto old_bb = builder->GetInsertBlock();
            builder->SetInsertPoint(comp_bb);
            //  create symbols for parameters
            idx = 0;
            for (auto arg = function->arg_begin(); arg != function->arg_end(); ++arg)
            {
                // argument type check is done in call_function
                auto arg_name = arg->getName();
                llvm::Value *arg_val = arg;
                auto full_type = para_type_list[idx];
                auto symbol = ir::Symbol::Get(full_type, arg_name);
                symbol->Store(arg_val);
                comp_block.DefineSymbol(arg_name, symbol);
                ++idx;
            }
            // parse statements
            if (!generate_code.at("compound_statement")(comp_stat, comp_block))
                return generator.Error("[ir\\fun-def] fail to generate statements block.");

            builder->SetInsertPoint(old_bb);

            // verify function
            std::string err_str;
            llvm::raw_string_ostream es(err_str);
            bool function_broken = llvm::verifyFunction(*function, &es);
            es.flush();
            std::cout << "\n[generator] function verification result: "
                      << (function_broken ? "wrong" : "correct") << std::endl;
            if (function_broken)
            {
                std::cout << "[ir] Error message:\n"
                          << err_str << std::endl;
                return false;
            }
            return true;
        }));

    // declaration
    generate_code.insert(std::pair<std::string, std::function<bool(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "declaration_list",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> bool {
            for (auto decl : node->children)
            {
                if (!generate_code.at("declaration")(decl, block))
                    return false;
            }
            return true;
        }));
    generate_code.insert(std::pair<std::string, std::function<bool(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "declaration",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> bool {
            auto decl_spec = node->children[0];
            // [not implement] 'const'/'static' yet
            auto root_type_str = decl_spec->getNameChild("type_specifier")->value;
            auto base_type = ParseBaseType(decl_spec, block);
            if (!base_type)
            {
                return false;
            }

            auto init_decl_list = node->children[1];
            for (auto child : init_decl_list->children)
            {
                // [not implement] 'pointer' yet
                // [not implement] 'array' yet
                auto &id_name = child->getNameChild("identifier")->value;
                std::vector<ir::RootType *> type_stack;
                type_stack.push_back(base_type);
                auto full_type = ir::Type::Get(type_stack);
                auto symbol = ir::Symbol::Get(full_type, id_name);
                // init_val = decl_type->Allocate(id_name);
                // auto init_val = builder->CreateAlloca(decl_type, nullptr, id_name);
                if (child->type == "init_declarator")
                {
                    // can be initializer_list or expression
                    // [not implement] 'initializer_list'
                    auto expr = child->children[1];
                    auto assign_value = resolve_symbol.at(expr->type)(expr, block)->RValue()->value;
                    if (!symbol->Store(assign_value))
                    {
                        return generator.Error("[ir\\decl] can't store value to symbol.");
                    }
                }
                // if init_val not correct
                if (!symbol->IsValid())
                {
                    return generator.Error("[ir\\decl] created symbol is not valid.");
                }

                // if variable already exists, error
                if (!block.DefineSymbol(id_name, symbol))
                {
                    return generator.Error("[ir\\decl] variable exits.");
                }
            }
            return true;
        }));

    // [flow control]
    generate_code.insert(std::pair<std::string, std::function<bool(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "if_else_statement",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> bool {
            auto &children = node->children;
            auto expr = children[0];

            // cond_value: int*
            auto cond_value = resolve_symbol.at("expression")(expr, block)->RValue()->value;
            if (!cond_value)
                return false;

            // int <- int*
            cond_value = builder->CreateLoad(cond_value);
            // float <- int
            cond_value = builder->CreateSIToFP(cond_value, llvm::Type::getFloatTy(*context));
            // bool <- float
            cond_value = builder->CreateFCmpONE(
                cond_value,
                llvm::ConstantFP::get(*context, llvm::APFloat(0.0f)),
                "cond-value");
            llvm::Function *block_fun = builder->GetInsertBlock()->getParent();
            // then block
            llvm::BasicBlock *true_block = llvm::BasicBlock::Create(
                *context,
                llvm::Twine("ture_block"),
                block_fun);
            // else block
            llvm::BasicBlock *false_block = llvm::BasicBlock::Create(
                *context,
                llvm::Twine("false_block"));
            // merge block
            llvm::BasicBlock *merge_block = llvm::BasicBlock::Create(
                *context,
                llvm::Twine("merge_block"));

            builder->CreateCondBr(cond_value,
                                  true_block,
                                  false_block);

            // Emit then llvm::Value.
            auto true_stat = children[1];
            ir::Block true_b(&block);
            auto old_bb = builder->GetInsertBlock();
            builder->SetInsertPoint(true_block);
            if (!generate_code.at("compound_statement")(true_stat, true_b))
                return false;

            builder->CreateBr(merge_block);
            // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
            true_block = builder->GetInsertBlock();
            builder->SetInsertPoint(old_bb);

            block_fun->getBasicBlockList().push_back(false_block);

            // Emit else block.
            auto false_stat = children[2];
            ir::Block false_b(&block);
            old_bb = builder->GetInsertBlock();
            builder->SetInsertPoint(false_block);
            if (!generate_code.at("compound_statement")(false_stat, false_b))
                return false;

            builder->CreateBr(merge_block);
            // Codegen of 'Else' can change the current block, update ElseBB for the PHI.
            false_block = builder->GetInsertBlock();
            builder->SetInsertPoint(old_bb);

            // Emit merge block.
            block_fun->getBasicBlockList().push_back(merge_block);
            builder->SetInsertPoint(merge_block);
            return true;
        }));
    // [return]
    generate_code.insert(std::pair<std::string, std::function<bool(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "return_expr",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> bool {
            auto expr_node = node->children[0];
            auto ret_symbol = resolve_symbol.at("expression")(expr_node, block);
            // not check yet
            return !builder->CreateRet(ret_symbol->RValue()->value)
                       ? generator.Error("[ir\\ret] can't create return instruction.")
                       : true;
        }));

    // [assignment]
    // [not implement] type check
    generate_code.insert(std::pair<std::string, std::function<bool(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "assignment_expression",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> bool {
            auto lhs_node = node->children[0];
            auto lhs_symbol = resolve_symbol.at(lhs_node->type)(lhs_node, block);
            if (!lhs_symbol)
            {
                return false;
            }

            auto rhs_node = node->children[1];
            auto rhs_symbol = resolve_symbol.at(rhs_node->type)(rhs_node, block);
            if (!rhs_symbol)
            {
                return false;
            }

            return lhs_symbol->Store(rhs_symbol->RValue()->value);
        }));

    // basic type

    // [function call]
    // [not implement] '.'
    resolve_symbol.insert(std::pair<std::string, std::function<std::shared_ptr<ir::Symbol>(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "function_call",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> std::shared_ptr<ir::Symbol> {
            auto &id_name = node->children[0]->value;
            auto fun = module->getFunction(id_name);
            if (!fun)
            {
                generator.Error("[ir\\fun-call] calling a not defined function.");
                return nullptr;
            }

            auto arg_expr_list = node->children[1];
            std::vector<llvm::Value *> arg_list;
            // load arguments
            for (auto arg : arg_expr_list->children)
            {
                auto arg_val = resolve_symbol.at(arg->type)(arg, block)->RValue()->value;
                if (!arg_val)
                {
                    return nullptr;
                }
                arg_list.push_back(arg_val);
            }
            auto fun_type = fun->getFunctionType();
            // check if argument's num == parameter's num
            if (fun_type->getNumParams() != arg_list.size())
            {
                generator.Error("[ir\\fun-call] number of arguments not match.");
                return nullptr;
            }

            // check if argument's type == parameter's type
            for (unsigned i = 0; i < fun_type->getNumParams(); ++i)
            {
                llvm::Type *para_type = fun_type->getFunctionParamType(i);
                auto arg = arg_list[i];
                // [not implement] detail type check
                if (para_type != arg->getType())
                {
                    generator.Error("[ir\\fun-call] parameter type not match.");
                    return nullptr;
                }
            }

            auto val = builder->CreateCall(fun, arg_list, "call_" + id_name);
            return nullptr;
        }));

    // init resolve value map
    // [expression]
    resolve_symbol.insert(std::pair<std::string, std::function<std::shared_ptr<ir::Symbol>(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "expression",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> std::shared_ptr<ir::Symbol> {
            auto child = node->children[0];
            return resolve_symbol.at(child->type)(child, block);
        }));
    resolve_symbol.insert(std::pair<std::string, std::function<std::shared_ptr<ir::Symbol>(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "int",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> std::shared_ptr<ir::Symbol> {
            auto real_val = atoi(node->value.c_str());
            auto val = llvm::ConstantInt::get(*context, llvm::APInt(32, real_val, true));
            auto type = ir::Type::GetConstantType("int");
            auto symbol = ir::Symbol::GetConstant(type, val);
            return symbol;
        }));
    resolve_symbol.insert(std::pair<std::string, std::function<std::shared_ptr<ir::Symbol>(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "float",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> std::shared_ptr<ir::Symbol> {
            auto real_val = atof(node->value.c_str());
            auto val = llvm::ConstantFP::get(*context, llvm::APFloat(real_val));
            auto type = ir::Type::GetConstantType("int");
            auto symbol = ir::Symbol::GetConstant(type, val);
            return symbol;
        }));
    resolve_symbol.insert(std::pair<std::string, std::function<std::shared_ptr<ir::Symbol>(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "char",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> std::shared_ptr<ir::Symbol> {
            auto real_val = atoi(node->value.c_str());
            auto val = llvm::ConstantInt::get(*context, llvm::APInt(8, real_val, false));
            auto type = ir::Type::GetConstantType("int");
            auto symbol = ir::Symbol::GetConstant(type, val);
            return symbol;
        }));
    resolve_symbol.insert(std::pair<std::string, std::function<std::shared_ptr<ir::Symbol>(std::shared_ptr<ast::Node>, ir::Block &)>>(
        "identifier",
        [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> std::shared_ptr<ir::Symbol> {
            auto &symbol_name = node->value;
            auto symbol = block.getSymbol(symbol_name);
            if (!symbol)
            {
                return symbol->Error("cannot find such identifier.");
            }
            return symbol;
        }));
}

bool ir::Generator::Generate(std::shared_ptr<ast::Node> &object)
{
    auto &generate_code = this->generate_code;
    try
    {
        // Main loop
        // Create infrastructure
        ir::CreateIrUnit();
        ir::Block global;
        auto &root = object;
        auto &type = root->type;
        if (type != "translation_unit")
        {
            throw "\n[ir] error: type is not translation_unit\n";
        }

        // Generate ir from a tree
        auto res = generate_code.at(type)(root, global);
        if (!res)
        {
            throw "\n[ir] error at parsing a unit.\n";
        }

        // Print ir
        std::string err_str;
        llvm::raw_string_ostream es(err_str);
        bool module_broken = llvm::verifyModule(*module, &es);
        es.flush();
        std::cout << "\n[ir] module verification result: "
                  << (module_broken ? "wrong" : "correct") << std::endl;
        if (module_broken)
        {
            std::cout << "[ir] Error message:\n"
                      << err_str << std::endl;
            return false;
        }
        else
        {
            return true;
        }
    }
    catch (const char *error)
    {
        // if an ast errors when generating IR
        std::cout << error;
        module->print(llvm::errs(), nullptr); // print error msg
        return false;
    }
}

// [IR]
void ir::CreateIrUnit()
{
    context = llvm::make_unique<llvm::LLVMContext>();
    builder = llvm::make_unique<llvm::IRBuilder<>>(*context);
    module = llvm::make_unique<llvm::Module>("my JIT", *context);
}

// [Block]
ir::Block *ir::Block::GetSymbolBlock(const std::string &name)
{
    Block *block = this;
    while (block)
    {
        if (block->HasSymbol(name))
            break;
    }
    return block;
}
std::shared_ptr<ir::Symbol> ir::Block::getSymbol(const std::string &name)
{
    Block *node = this;
    while (node)
    {
        if (this->HasSymbol(name))
            return node->SymbolTable.at(name);
        node = node->parent;
    }
    return nullptr;
}
bool ir::Block::DefineSymbol(const std::string &name, std::shared_ptr<ir::Symbol> val)
{
    if (this->HasSymbol(name))
        return false;
    else
    {
        this->SymbolTable[name] = std::move(val);
        return true;
    }
}
bool ir::Block::SetSymbol(const std::string &name, std::shared_ptr<ir::Symbol> val)
{
    if (!this->HasSymbol(name))
        return false;
    // [not implement] check mutable
    else
    {
        this->SymbolTable[name] = std::move(val);
        return true;
    }
}
bool ir::Block::HasSymbol(const std::string &name)
{
    return this->SymbolTable.count(name) != 0;
}