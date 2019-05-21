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