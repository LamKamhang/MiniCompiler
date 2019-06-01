#include <generator.h>
#include <ir.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <type.h>
#include <memory>

template <typename... T>
std::shared_ptr<ir::Type> getType(T... args)
{
	auto &context = ir::Context;
	auto &typeMap = ir::TypeMap;
	return llvm::Type::getInt32Ty(context);
}

llvm::Value *ir::LogError(const char *str)
{
	std::cout << str << std::endl;
	return nullptr;
}

void ir::Generator::init()
{
	static auto &table = ir::Generator::table;
	table.insert({"function_definition",
				  [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {
					  const auto &children = node->children;
					  const auto type_spec = children[0]->children[0];
					  const auto ret_type = ir::getType(type_spec->children[0]->value);
					  if (!ret_type)
					  {
						  return ir::LogError("error at function return type");
					  }

					  const std::string &id = children[1]->children[0]->children[0]->value;
					  auto para_type = children[1]->children[0]->children[1]->children[0]->children[0]->children[0]->if (!direct_decl);
					  {
						  return ir::LogError("error at function id");
					  }
					  auto comp_stat = table["compound_statement"](children[2]);
					  if (!comp_stat)
					  {
						  return ir::LogError("error at function statement");
					  }
				  }});
	table.insert({"parameter_declaration",
				  [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {}});
	table.insert({"declaration_specifiers_p",
				  [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {}});
	table.insert({"compound_statement",
				  [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {}});
	table.insert({"jump_statement",
				  [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {}});
	table.insert({"primary_expression",
				  [&](std::shared_ptr<ast::Node> node) -> llvm::Value * {}});
}

llvm::Value *ir::Generator::generate(
	std::vector<std::shared_ptr<ast::Node>> &objects)
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
	ir::TypeMap = std::make_unique<std::map<std::string, ir::Type>>();
}