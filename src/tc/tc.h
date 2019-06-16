#pragma once
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/Support/raw_sha1_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <system_error>
#include <utility>
#include <vector>
namespace tc
{

bool targetGenerate(const std::string &filename);
bool targetGenerate(std::ostream &os);
} // namespace tc