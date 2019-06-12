#include "type.h"
#include <ir.h>
#include <stack>
#include <string>
namespace ir
{
class Symbol
{
public:
    std::string name;
    llvm::Value *value;
    Symbol();
};
} // namespace ir