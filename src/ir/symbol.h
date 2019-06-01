#include <ir.h>

namespace ir
{
class SymbolTable : public std::map<std::string, llvm::Value *>
{
};
} // namespace ir