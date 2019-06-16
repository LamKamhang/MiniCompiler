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
    template <typename T>
    llvm::Value *CastTo(const T *type, llvm::Value *value);
    static ir::FloatTy *Get(int bits, bool is_const);
    static llvm::Type *GetBitType(int bits);
};
} // namespace ir