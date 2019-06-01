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
                      const auto &children = node->children;
                      const auto type_spec = children[0]->children[0];
                      auto ret_type = ir::Type::getInt32Ty(*ir::Context);
                      if (!ret_type)
                      {
                          return ir::Generator::LogError("error at function return type");
                      }

                      const std::string &id = children[1]->children[0]->children[0]->value;
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