#pragma once
#include "type.h"

namespace ir
{
class FloatTy : public BaseType
{
private:
    FloatTy() = default;

public:
    FloatTy(int bits, bool is_const = false);

public:
    int bits;

    std::string TyInfo();
    llvm::Value *CastTo(ir::RootType *type, llvm::Value *value);
    static ir::FloatTy *Get(int bits, bool is_const);
    static llvm::Type *GetBitType(int bits);
};
} // namespace ir