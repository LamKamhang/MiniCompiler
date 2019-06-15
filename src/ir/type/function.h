#pragma once
#include "type/type.h"
#include "vector"
namespace ir
{
class FunctionTy : public ir::RootType
{
protected:
    FunctionTy() = default;

public:
    FunctionTy(bool defined, const std::string &name);
    bool defined;
    std::string name;
    std::shared_ptr<ir::Type> ret_type;
    std::vector<std::shared_ptr<ir::Type>> para_type;
    llvm::Value *value;
    llvm::Value *Allocate(const std::string &name);
    std::string TyInfo();
    static std::shared_ptr<ir::FunctionTy> Get(bool defined, const std::string &name, std::vector<std::shared_ptr<ir::Type>> &types);
};
} // namespace ir