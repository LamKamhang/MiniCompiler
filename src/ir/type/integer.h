#pragma once
#include "type.h"

namespace ir
{
class IntegerTy : public BaseType
{
private:
    IntegerTy() = default;

public:
    IntegerTy(int bits, bool is_sign, bool is_const = false);

public:
    int bits;
    bool is_sign;

    std::string TyInfo();
    llvm::Value *CastTo(ir::RootType *type, llvm::Value *value);
    static ir::IntegerTy *Get(int bits, bool is_sign, bool is_const);
    static llvm::Type *GetBitType(int bits);
};
} // namespace ir