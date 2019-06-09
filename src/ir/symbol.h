#include "type.h"
#include <ir.h>
#include <stack>
namespace ir
{
class Symbol
{
public:
    std::stack<ir::Type *> ty_stack;
    Symbol();
};
} // namespace ir