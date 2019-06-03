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
                      auto type_spec = node->getNameChild("type_specifier");
                      auto ret_type = ir::Type::getInt32Ty(*ir::Context);
                      if (!ret_type)
                      {
                          return ir::Generator::LogError("error at function return type");
                      }
                      auto dire_decl = node->getNameChild("direct_declarator");
                      const std::string &id = dire_decl->children[0]->getNameChild("identifier")->value;
                      auto para_list = dire_decl->getNameChild("parameter_list");
                      for (auto para : para_list->children)
                      {
                          const std::string &para_id = para->getNameChild("identifier")->value;
                          auto type_spec = para->getNameChild("type_specifier")->children[0]->value;
                          auto type = ir::Block::getCustomType(block, type_spec);
                      }
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