#include <ir.h>
#include <llvm/IR/Value.h>

namespace ir
{
class Value : public llvm::Value
{

    llvm::Value deRef();
};
} // namespace ir