#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <block.h>
#include <generator.h>
#include <ir.h>
#include <type.h>
#include <value.h>
#include <memory>
#include <exception>

llvm::Value *ir::Generator::LogError(const char *str)
{
    std::cout << str << std::endl;
    return nullptr;
}

void ir::Generator::init()
{
    static auto &table = ir::Generator::table;
    table.insert({"function_definition",
                  [&](std::shared_ptr<ast::Node> node, const ir::Block &block) -> llvm::Value * {
                      // function return type
                      auto func_decl = node;
                      auto type_spec = func_decl->children[0]->getNameChild("type_specifier");
                      auto ret_type = ir::Block::getCustomType(block, type_spec);

                      //  function name
                      auto decl = func_decl->children[1];
                      const std::string &fun_id = decl->children[0]->getNameChild("identifier")->value;

                      //   compound statements
                      auto comp_stat = func_decl->children[2];
                      const ir::Block comp_block;

                      // parameter list
                      auto para_list = decl->children[1];
                      std::vector<llvm::Type *> para_type;
                      for (auto para_decl : para_list->children)
                      {
                          //   parameter id
                          const std::string &para_id = para_decl->getNameChild("identifier")->value;

                          // parameter type
                          auto type_spec = para_decl->getNameChild("type_specifier")->children[0]->value;
                          auto type = ir::Block::getCustomType(block, type_spec);
                          para_type.push_back(type);
                          comp_block.SymbolTable[para_id] = ;
                      }

                      // parse statements
                      auto statments = table[comp_stat->type](comp_stat, comp_block);
                      if (!statments)
                      {
                      }
                  }});
    table.insert({"compound_statement",
                  [&](std::shared_ptr<ast::Node> node, const ir::Block &block) -> llvm::Value * {

                  }});
}

llvm::Value *ir::Generator::generate(
    std::vector<std::shared_ptr<ast::Node>> &objects)
{
    auto &table = ir::Generator::table;
    try
    {
        // Main loop
        for (auto i = objects.begin(); i != objects.end(); ++i)
        {
            // Create infrastructure
            ir::createIrUnit();
            ir::Block global;
            auto &root = (*i);
            auto &type = root->type;
            if (type != "translation_unit")
            {
                throw "error, type is not translation_unit";
            }

            for (auto node : root->children)
            {
                auto res = table.at(type)(node, global);
                if (!res)
                {
                    res->print(llvm::errs()); // print error msg if has
                    throw "error at parsing one node";
                }
            }
        }
    }
    catch (const std::string &error)
    {
        std::cout << error << std::endl;
    }

    // if an ast errors when generating IR
    ir::Module->print(llvm::errs(), nullptr); // print error msg
}
void ir::createIrUnit()
{
    ir::Context = std::make_unique<llvm::LLVMContext>();
    ir::Builder = std::make_unique<llvm::IRBuilder<>>("my JIT", *ir::Context);
    ir::Module = std::make_unique<llvm::Module>();
}
void ir::CustomValue::setType(ir::Type *type)
{
    this->type = type;
    this->pos = this->type->typeStack.size();
}