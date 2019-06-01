#include <ir.h>
#include <../ast/ast.h>
namespace ir
{
class Generator
{
public:
    static std::map<std::string, std::function<llvm::Value *(std::shared_ptr<ast::Node>)>> table;
    Generator() = default;
    void init();
    llvm::Value *generate(std::vector<std::shared_ptr<ast::Node>> &root);
};
llvm::Value *LogError(const char *str);
} // namespace ir