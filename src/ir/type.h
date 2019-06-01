#include <ir.h>
#include <llvm/IR/Type.h>
#include <stack>
#include <string>
#include <map>
namespace ir
{
class Type : public llvm::Type
{
public:
    std::stack<llvm::Type> typeStack;
    Type() = default;
};

template <typename... T>
std::shared_ptr<ir::Type> getType(T... args);
} // namespace ir