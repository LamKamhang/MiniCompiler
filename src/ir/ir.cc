#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
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
                  [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> llvm::Value * {
                      // function return type
                      auto func_decl = node;
                      auto type_spec = func_decl->children[0]->getNameChild("type_specifier")->children[0]->value;
                      auto ret_type = ir::Block::getCustomType(block, type_spec);

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
                          const std::string &para_id = para_decl->getNameChild("identifier")->value;
                          para_name.push_back(para_id);

                          // parameter type
                          auto type_spec = para_decl->getNameChild("type_specifier")->children[0]->value;
                          auto type = ir::Block::getCustomType(block, type_spec);
                          para_type.push_back(type);
                      }

                      // create function prototype
                      llvm::FunctionType *function_type = llvm::FunctionType::get(ret_type, para_type, false);
                      llvm::Function *function = llvm::Function::Create(function_type, llvm::GlobalValue::LinkageTypes::CommonLinkage, fun_name, ir::Module.get());
                      // set parameter name
                      unsigned idx = 0;
                      for (auto &arg : function->args())
                      {
                          arg.setName(para_name[idx++]);
                      }
                      if (!function)
                      {
                          return ir::Generator::LogError("fail to generate function.");
                      }
                      if (!function->empty())
                      {
                          return ir::Generator::LogError("function can not be redefined.");
                      }

                      // compound statements
                      auto comp_stat = func_decl->children[2];
                      ir::Block comp_block(&block);
                      // use a new basic block
                      comp_block.basic_block = llvm::BasicBlock::Create(*ir::Context, fun_name + "_block", function);
                      // record function parameters
                      for (auto &arg : function->args())
                      {
                          comp_block.SymbolTable[arg.getName()] = &arg;
                      }
                      // parse statements
                      auto statments = table[comp_stat->type](comp_stat, comp_block);
                      if (!statments)
                      {
                          return ir::Generator::LogError("fail to generate statements block.");
                      }

                      llvm::verifyFunction(*function);
                      return function;
                  }});
    table.insert({"retun",
                  [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> llvm::Value * {
                      return (llvm::Value *)1;
                  }});
    table.insert({"compound_statement",
                  [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> llvm::Value * {
                      ir::Builder->SetInsertPoint(block.basic_block);
                      for (auto stat : node->children)
                      {
                          if (!table.at(stat->type)(stat, block))
                          {
                              return nullptr;
                          }
                      }
                      return (llvm::Value *)1;
                  }});

    table.insert({"translation_unit",
                  [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> llvm::Value * {
                      for (auto child : node->children)
                      {
                          if (!table.at(child->type)(child, block))
                          {
                              return nullptr;
                          }
                      }
                      return (llvm::Value *)1;
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

            auto res = table.at(type)(root, global);
            if (!res)
            {
                res->print(llvm::errs()); // print error msg if has
                throw "error at parsing a unit.";
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