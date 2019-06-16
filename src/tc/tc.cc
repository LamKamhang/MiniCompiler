#include "tc.h"
#include "../ir/ir.h"

using namespace llvm;

bool tc::targetGenerate(std::ostream &os) {
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  auto target_triple = llvm::sys::getDefaultTargetTriple();
  module->setTargetTriple(target_triple);

  std::string error;
  auto target = llvm::TargetRegistry::lookupTarget(target_triple, error);

  // Print an error and exit if we couldn't find the requested target.
  // This generally occurs if we've forgotten to initialise the
  // TargetRegistry or we have a bogus target triple.
  if (!target) {
    llvm::errs() << error;
    return false;
  }

  auto cpu = "generic";
  auto features = "";

  llvm::TargetOptions opt;
  auto rm = llvm::Optional<llvm::Reloc::Model>();
  auto target_machine =
      target->createTargetMachine(target_triple, cpu, features, opt, rm);

  module->setDataLayout(target_machine->createDataLayout());

  llvm::legacy::PassManager pass;
  auto file_type = llvm::TargetMachine::CGFT_ObjectFile;

  std::string out;
  llvm::raw_string_ostream ros(out);
  llvm::buffer_ostream bos(ros);
#if LLVM_VERSION_MAJOR == 6
  if (target_machine->addPassesToEmitFile(pass, bos, file_type))
#else
  if (target_machine->addPassesToEmitFile(pass, bos, nullptr, file_type))
#endif
  {
    llvm::errs() << "target_machine can't emit a file of this type";
    return false;
  }

  pass.run(*module);
  ros.flush();
  std::cout << "----" << std::endl;
  std::cout << out << ":" << std::endl;
  std::cout << "---" << std::endl;
  os << out;

  llvm::outs() << "\n[tc] generate a target code file.\n";

  return true;
}

bool tc::targetGenerate(const std::string &filename) {
  // Initialize the target registry etc.
  InitializeAllTargetInfos();
  InitializeAllTargets();
  InitializeAllTargetMCs();
  InitializeAllAsmParsers();
  InitializeAllAsmPrinters();

  auto TargetTriple = sys::getDefaultTargetTriple();
  module->setTargetTriple(TargetTriple);

  std::string Error;
  auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);

  // Print an error and exit if we couldn't find the requested target.
  // This generally occurs if we've forgotten to initialise the
  // TargetRegistry or we have a bogus target triple.
  if (!Target) {
    errs() << Error;
    return 1;
  }

  auto CPU = "generic";
  auto Features = "";

  TargetOptions opt;
  auto RM = Optional<Reloc::Model>();
  auto TheTargetMachine =
      Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

  module->setDataLayout(TheTargetMachine->createDataLayout());

  std::error_code EC;
  raw_fd_ostream dest(filename, EC, sys::fs::F_None);

  if (EC) {
    errs() << "Could not open file: " << EC.message();
    return 1;
  }

  legacy::PassManager pass;
  auto FileType = TargetMachine::CGFT_ObjectFile;

  if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
    errs() << "TheTargetMachine can't emit a file of this type";
    return 1;
  }

  pass.run(*module);
  dest.flush();

  outs() << "Wrote " << filename << "\n";

  return 0;
}