#include <ir.h>
#include <../util/ast.h>
namespace ir
{
class Generator
{
public:
    static std::map<std::string, std::function<llvm::Value *(std::shared_ptr<ast::Node>)>> table;
    Generator() = default;
    void init();
    void ir::Generator::createModule();
    llvm::Value *generate(std::vector<std::shared_ptr<ast::Node>> &root);
};
} // namespace ir