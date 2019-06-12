#pragma once
#include "ir.h"
#include <llvm/IR/Type.h>
#include <map>
#include <stack>
#include <string>
namespace ir
{
virtual class BaseType
{
public:
    llvm::Type *_ty;
    bool constant;
    BaseType(llvm::Type *type, bool constant = false) : _ty(type), constant(constant){};
    static llvm::Type *getCustomType(const std::string &type);
};
class PureType : public BaseType
{
public:
    PureType(){};
    virtual as();
};
class Type
{
private:
    std::stack<BaseType *> _stack;

public:
    Type()
}
} // namespace ir