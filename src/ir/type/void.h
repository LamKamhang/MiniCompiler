#pragma once
#include "type.h"
namespace ir
{
class VoidTy : public ir::BaseType
{

public:
    VoidTy() : ir::BaseType(llvm::Type::getVoidTy(*context), ir::TypeName::Void, true){};
    std::string TyInfo();
    static ir::VoidTy *Get();
};
} // namespace ir