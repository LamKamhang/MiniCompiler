#include <ir.h>
namespace ir
{
class Generator
{
public:
    static std::map<std::string, std::function<llvm::Value *(std::shared_ptr<ast::Node>)>> table;
    Generator() = default;
    void init();
};
} // namespace ir