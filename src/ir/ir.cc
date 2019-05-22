#include <ir.h>
#include <memory>
#include <llvm/IR/IRBuilder.h>
#include <generator.h>

void ir::Generator::init()
{
    auto &table = ir::Generator::table;
    table.insert({"function_definition", [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {
                  }});
    table.insert({"function_specifiers_i", [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {
                  }});
    table.insert({"direct_declarator", [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {
                  }});
    table.insert({"compound_statement", [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {
                  }});
    table.insert({"jump_statement", [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {
                  }});
    table.insert({"primary_expression", [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {
                  }});
    table.insert({"jump_statement", [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {
                  }});
}

void ir::Generator::createModule()
{
    const std::vector<std::shared_ptr<ast::Node>> res;
    ir::Context = std::make_unique<llvm::LLVMContext>();
    ir::Builder = std::make_unique<llvm::IRBuilder<>>(*ir::Context);
    ir::Module = std::make_unique<llvm::Module>();
}
llvm::Value *ir::Generator::generate(std::vector<std::shared_ptr<ast::Node>> &objects)
{
    auto &table = ir::Generator::table;
    for (auto i = objects.begin(); i != objects.end(); ++i)
    {
        auto &node = (*i);
        auto &type = node->type;
        table.at(type)(node)->print(llvm::errs()); // print err msg if has
    }
    ir::Module->print(llvm::errs(), nullptr);
}