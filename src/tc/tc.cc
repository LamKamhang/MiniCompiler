#include <tc/tc.h>
#include <ir/ir.h>

bool tc::targetGenerate(std::ostream &os)
{

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
    if (!target)
    {
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
    if (target_machine->addPassesToEmitFile(pass, bos, nullptr, file_type))
    {
        llvm::errs() << "target_machine can't emit a file of this type";
        return false;
    }

    pass.run(*module);
    ros.flush();
    os << out;

    llvm::outs() << "\n[tc] generate a target code file.\n";

    return true;
}