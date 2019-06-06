#include <block.h>
#include <exception>
#include <generator.h>
#include <ir.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <memory>
#include <stdlib.h>
#include <type.h>
#include <value.h>

std::unique_ptr<llvm::LLVMContext> Context;
std::unique_ptr<llvm::IRBuilder<>> Builder;
std::unique_ptr<llvm::Module> Module;

llvm::Value *ir::Generator::LogError(const char *str) {
  std::cout << str << std::endl;
  return nullptr;
}

void ir::Generator::init() {
  auto &table = this->table;
  table.insert(std::pair<std::string,
                         std::function<llvm::Value *(std::shared_ptr<ast::Node>,
                                                     ir::Block &)>>(
      "function_definition",
      [&](std::shared_ptr<ast::Node> node, ir::Block &block) -> llvm::Value * {
        // function return type
        auto func_decl = node;
        auto type_spec =
            func_decl->children[0]->getNameChild("type_specifier")->value;
        auto ret_type = ir::Block::getCustomType(block, type_spec);

        //  function name
        auto decl = func_decl->children[1];
        const std::string &fun_name =
            decl->children[0]->getNameChild("identifier")->value;

        // parameter list
        auto para_list = decl->children[1];
        std::vector<llvm::Type *> para_type;
        std::vector<std::string> para_name;
        for (auto para_decl : para_list->children) {
          //   parameter id
          const std::string &para_id =
              para_decl->getNameChild("identifier")->value;
          para_name.push_back(para_id);

          // parameter type
          auto type_spec = para_decl->getNameChild("type_specifier")->value;
          auto type = ir::Block::getCustomType(block, type_spec);
          para_type.push_back(std::move(type));
        }

        // create function prototype
        llvm::FunctionType *function_type =
            llvm::FunctionType::get(ret_type, para_type, false);
        llvm::Function *function = llvm::Function::Create(
            function_type, llvm::GlobalValue::ExternalLinkage, fun_name,
            Module.get());
        // set parameter name
        unsigned idx = 0;
        for (auto &arg : function->args()) {
          arg.setName(para_name[idx++]);
        }
        if (!function) {
          return ir::Generator::LogError("fail to generate function.");
        }
        if (!function->empty()) {
          return ir::Generator::LogError("function can not be redefined.");
        }

        // compound statements
        auto comp_stat = func_decl->children[2];
        ir::Block comp_block(&block);
        // use a new basic block
        comp_block.basic_block =
            llvm::BasicBlock::Create(*Context, fun_name + "_block", function);
        // record function parameters
        for (auto &arg : function->args()) {
          comp_block.SymbolTable[arg.getName()] = &arg;
        }
        // parse statements
        auto statments = table.at(comp_stat->type)(comp_stat, comp_block);
        if (!statments) {
          return ir::Generator::LogError("fail to generate statements block.");
        }

        bool function_err = llvm::verifyFunction(*function);
        std::cout << "\n[generator] function verification result: "
                  << (function_err ? "wrong" : "correct") << std::endl;
        if (function_err) {
          Module->print(llvm::errs(), nullptr);
        }
        return function;
      }));

  table.insert(
      std::pair<std::string, std::function<llvm::Value *(
                                 std::shared_ptr<ast::Node>, ir::Block &)>>(
          "return_expr",
          [&](std::shared_ptr<ast::Node> node,
              ir::Block &block) -> llvm::Value * {
            auto val = atoi(node->children[0]->value.c_str());
            Builder->CreateRet(
                llvm::ConstantInt::get(*Context, llvm::APInt(32, val, true)));
            return (llvm::Value *)1;
          }));
  table.insert(
      std::pair<std::string, std::function<llvm::Value *(
                                 std::shared_ptr<ast::Node>, ir::Block &)>>(
          "statement_list",
          [&](std::shared_ptr<ast::Node> node,
              ir::Block &block) -> llvm::Value * {
            for (auto stat : node->children) {
              if (!table.at(stat->type)(stat, block)) {
                return nullptr;
              }
            }
            return (llvm::Value *)1;
          }));
  table.insert(
      std::pair<std::string, std::function<llvm::Value *(
                                 std::shared_ptr<ast::Node>, ir::Block &)>>(
          "compound_statement",
          [&](std::shared_ptr<ast::Node> node,
              ir::Block &block) -> llvm::Value * {
            auto lastBlock = Builder->GetInsertBlock();
            Builder->SetInsertPoint(block.basic_block);
            for (auto stat : node->children) {
              if (!table.at(stat->type)(stat, block)) {
                return nullptr;
              }
            }
            Builder->SetInsertPoint(lastBlock);
            return (llvm::Value *)1;
          }));

  table.insert(
      std::pair<std::string, std::function<llvm::Value *(
                                 std::shared_ptr<ast::Node>, ir::Block &)>>(
          "translation_unit",
          [&](std::shared_ptr<ast::Node> node,
              ir::Block &block) -> llvm::Value * {
            for (auto child : node->children) {
              if (!table.at(child->type)(child, block)) {
                return nullptr;
              }
            }
            return (llvm::Value *)1;
          }));
  table.insert(
      std::pair<std::string, std::function<llvm::Value *(
                                 std::shared_ptr<ast::Node>, ir::Block &)>>(
          "int",
          [&](std::shared_ptr<ast::Node> node,
              ir::Block &block) -> llvm::Value * {
            auto val = atoi(node->children[0]->value.c_str());
            return llvm::ConstantInt::get(*Context, llvm::APInt(32, val, true));
          }));
 table.insert(
      std::pair<std::string, std::function<llvm::Value *(
                                 std::shared_ptr<ast::Node>, ir::Block &)>>(
          "float",
          [&](std::shared_ptr<ast::Node> node,
              ir::Block &block) -> llvm::Value * {
            auto val = atoi(node->children[0]->value.c_str());
            return llvm::ConstantFP::get(*Context,llvm::APFloat());
          }));   
}

llvm::Value *
ir::Generator::generate(std::vector<std::shared_ptr<ast::Node>> &objects) {
  auto &table = this->table;
  try {
    // Main loop
    for (auto i = objects.begin(); i != objects.end(); ++i) {
      // Create infrastructure
      ir::createIrUnit();
      ir::Block global;
      auto &root = (*i);
      auto &type = root->type;
      if (type != "translation_unit") {
        throw "error, type is not translation_unit";
      }

      // Generate ir from a tree
      auto res = table.at(type)(root, global);
      if (!res) {
        res->print(llvm::errs()); // print error msg if has
        throw "error at parsing a unit.";
      }

      // Print ir
      bool broken_debug_info = true;
      std::string err_str;
      llvm::raw_string_ostream es(err_str);
      bool module_broken = llvm::verifyModule(*Module, &es);
      es.flush();
      std::cout << "\n[main] module verification result: "
                << (module_broken ? "wrong" : "correct") << std::endl;
      if (module_broken) {
        std::cout << "Error message:\n" << err_str << std::endl;
      } else {
        std::string out_str;
        llvm::raw_string_ostream os(out_str);
        os << *Module;
        os.flush();
        std::cout << "\n[main] Generated IR:\n" + out_str << std::endl;
      }
    }
    return (llvm::Value *)1;
  } catch (char const *error) {
    // if an ast errors when generating IR
    Module->print(llvm::errs(), nullptr); // print error msg
    std::cout << error << std::endl;
  }
  return nullptr;
}

void ir::createIrUnit() {
  Context = llvm::make_unique<llvm::LLVMContext>();
  Builder = llvm::make_unique<llvm::IRBuilder<>>(*Context);
  Module = llvm::make_unique<llvm::Module>("my JIT", *Context);
}
void ir::CustomValue::setType(ir::Type *type) {
  this->type = type;
  this->pos = this->type->typeStack.size();
}