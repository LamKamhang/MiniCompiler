#include "../../src/ir/index.h"
#include "../../src/targetcode/tc.h"
#include <ast/ast.h>
#include <fstream>
#include <iostream>
#include <lib/json/json.h>
#include <llvm/IR/Verifier.h>
#include <memory>
#include <string>
#include <vector>

extern std::string cwd;

int tar_generate() {

  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  auto TargetTriple = llvm::sys::getDefaultTargetTriple();
  Module->setTargetTriple(TargetTriple);

  std::string Error;
  auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

  // Print an error and exit if we couldn't find the requested target.
  // This generally occurs if we've forgotten to initialise the
  // TargetRegistry or we have a bogus target triple.
  if (!Target) {
    llvm::errs() << Error;
    return 1;
  }

  auto CPU = "generic";
  auto Features = "";

  llvm::TargetOptions opt;
  auto RM = llvm::Optional<llvm::Reloc::Model>();
  auto TheTargetMachine =
      Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

  Module->setDataLayout(TheTargetMachine->createDataLayout());

  auto Filename = "output.o";
  std::error_code EC;
  llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::F_None);

  if (EC) {
    llvm::errs() << "Could not open file: " << EC.message();
    return 1;
  }

  llvm::legacy::PassManager pass;
  auto FileType = llvm::TargetMachine::CGFT_ObjectFile;

  if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
    llvm::errs() << "TheTargetMachine can't emit a file of this type";
    return 1;
  }

  pass.run(*Module);
  dest.flush();

  llvm::outs() << "Wrote " << Filename << "\n";

  return 0;
}

int main() {
  const std::string &path = "test/function_definition/";
  const std::string &file_name = "2";
  // Read json
  auto root = Json::parseJson(path + file_name + ".json");
  // Recover AST from Json::Value
  std::vector<std::shared_ptr<ast::Node>> forest;
  forest.push_back(ast::imports(root));
  // Generate IR form AST
  ir::Generator generator;
  auto res = generator.generate(forest);
  // Save IR to file
  std::string ir_code;
  llvm::raw_string_ostream ros(ir_code);
  ros << *Module;
  std::ofstream file(cwd + path + file_name + ".ll");
  file << ir_code;
  file.close();
  //
  tar_generate();
}