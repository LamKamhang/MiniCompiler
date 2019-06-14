#pragma once
#include "type.h"

namespace ir
{
class IntegerTy : public BaseType
{
public:
    int bits;
    bool is_sign;
    IntegerTy() = default;
    IntegerTy(int bits, bool is_sign, bool is_const = false);

    llvm::Value *allocate(const std::string &name);

    template <typename T>
    llvm::Value *castTo(const T *type, llvm::Value *value);
    static llvm::Type *get(int bits);
};
} // namespace ir