#include <ir.h>
#include <llvm/IR/Type.h>
#include <stack>
#include <string>
namespace ir
{
template <typename type, typename... T>
class Type : public llvm::Type
{
public:
    std::stack<llvm::Type> typeStack;
    Type(type, T... args);
};

class TypeManager
{
public:
    TypeManager();
    Type produce(std::string types[]);
};
} // namespace ir