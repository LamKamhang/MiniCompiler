#include <ir.h>
#include <llvm/IR/Type.h>
#include <memory>
#include <llvm/IR/IRBuilder.h>
#include <generator.h>

void ir::Generator::init()
{
    static auto &table = ir::Generator::table;
    table.insert({"function_definition", [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {
                      const auto &children = node->children;
                      auto decl_spec = table["declaration_specifiers_i"](children[0]);
                      if (!decl_spec)
                      {
                          return nullptr;
                      }
                      auto direct_decl = table["direct_declarator"](children[1]);
                      if (!direct_decl)
                      {
                          return nullptr;
                      }
                      auto comp_stat = table["compound_statement"](children[2]);
                      if (!comp_stat)
                      {
                          return nullptr;
                      }
                  }});
    table.insert({"declaration_specifiers_i", [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {
                      return llvm::Type::getInt32Ty(*ir::Context);
                  }});
    table.insert({"direct_declarator", [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {
                  }});
    table.insert({"parameter_declaration", [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {
                  }});
    table.insert({"declaration_specifiers_p", [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {
                  }});
    table.insert({"compound_statement", [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {
                  }});
    table.insert({"jump_statement", [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {
                  }});
    table.insert({"primary_expression", [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {
                  }});
}

llvm::Value *ir::Generator::generate(std::vector<std::shared_ptr<ast::Node>> &objects)
{
    auto &table = ir::Generator::table;
    for (auto i = objects.begin(); i != objects.end(); ++i)
    {
        auto &node = (*i);
        auto &type = node->type;
        auto res = table.at(type)(node);
        // if an ast errors when generating IR
        if (!res)
        {
            res->print(llvm::errs()); // print error msg if has
            break;
        }
    }
    ir::Module->print(llvm::errs(), nullptr); // print error msg
}
void ir::createModule()
{
    ir::Context = std::make_unique<llvm::LLVMContext>();
    ir::Builder = std::make_unique<llvm::IRBuilder<>>("my JIT", *ir::Context);
    ir::Module = std::make_unique<llvm::Module>();
}