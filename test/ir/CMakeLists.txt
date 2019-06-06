cmake_minimum_required(VERSION 2.8)

project(ir_test)
set(target ir_test)

set(source
    ../../src/ir/ir.cc
    ../../src/tc/tc.cc
    ../../src/ast/ast.cc
    ../../src/lib/json/jsoncpp.cc
    ../../src/util/json.cc
    main.cc
)

include_directories(${PROJECT_SOURCE_DIR}/../../src)
include_directories(${PROJECT_SOURCE_DIR}/../../src/ir)
include_directories(${PROJECT_SOURCE_DIR}/../../src/lib)
include_directories(${PROJECT_SOURCE_DIR}/../../src/util)
include_directories(${PROJECT_SOURCE_DIR}/../../src/ast)
include_directories(${PROJECT_SOURCE_DIR}/../../src/tc)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_executable(
    ${target}
    ${source}
)

# llvm lib
find_package(
    LLVM
    REQUIRED
    CONFIG
)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# llvm_map_components_to_libnames(llvm_libs
# LTO Passes ObjCARCOpts WindowsManifest FuzzMutate XCoreDisassembler XCoreCodeGen XCoreDesc XCoreInfo XCoreAsmPrinter SystemZDisassembler SystemZCodeGen SystemZAsmParser SystemZDesc SystemZInfo SystemZAsmPrinter SparcDisassembler SparcCodeGen SparcAsmParser SparcDesc SparcInfo SparcAsmPrinter PowerPCDisassembler PowerPCCodeGen PowerPCAsmParser PowerPCDesc PowerPCInfo PowerPCAsmPrinter NVPTXCodeGen NVPTXDesc NVPTXInfo NVPTXAsmPrinter MSP430CodeGen MSP430Desc MSP430Info MSP430AsmPrinter MipsDisassembler MipsCodeGen MipsAsmParser MipsDesc MipsInfo MipsAsmPrinter LanaiDisassembler LanaiCodeGen LanaiAsmParser LanaiDesc LanaiAsmPrinter LanaiInfo HexagonDisassembler HexagonCodeGen HexagonAsmParser HexagonDesc HexagonInfo BPFDisassembler BPFCodeGen BPFAsmParser BPFDesc BPFInfo BPFAsmPrinter ARMDisassembler ARMCodeGen ARMAsmParser ARMDesc ARMInfo ARMAsmPrinter ARMUtils AMDGPUDisassembler AMDGPUCodeGen AMDGPUAsmParser AMDGPUDesc AMDGPUInfo AMDGPUAsmPrinter AMDGPUUtils AArch64Disassembler AArch64CodeGen AArch64AsmParser AArch64Desc AArch64Info AArch64AsmPrinter AArch64Utils ObjectYAML
# Interpreter TableGen DlltoolDriver XRay OrcJIT MIRParser Symbolize DebugInfoPDB DebugInfoDWARF LibDriver Option Coroutines ipo Instrumentation Vectorize Linker IRReader AsmParser Coverage X86Disassembler X86AsmParser MCJIT ExecutionEngine RuntimeDyld X86CodeGen GlobalISel X86Desc X86Info MCDisassembler X86AsmPrinter X86Utils SelectionDAG AsmPrinter CodeGen Target ScalarOpts InstCombine TransformUtils BitWriter Analysis ProfileData Object MCParser MC DebugInfoCodeView DebugInfoMSF BitReader Core BinaryFormat Support Demangle
# WebAssemblyDisassembler WebAssemblyCodeGen WebAssemblyDesc WebAssemblyAsmPrinter WebAssemblyAsmParser WebAssemblyInfo
# )
llvm_map_components_to_libnames(llvm_libs
LTO Passes ObjCARCOpts WindowsManifest FuzzMutate TextAPI XCoreDisassembler XCoreCodeGen XCoreDesc XCoreInfo XCoreAsmPrinter WebAssemblyDisassembler WebAssemblyCodeGen WebAssemblyDesc WebAssemblyAsmPrinter WebAssemblyAsmParser WebAssemblyInfo SystemZDisassembler SystemZCodeGen SystemZAsmParser SystemZDesc SystemZInfo SystemZAsmPrinter SparcDisassembler SparcCodeGen SparcAsmParser SparcDesc SparcInfo SparcAsmPrinter PowerPCDisassembler PowerPCCodeGen PowerPCAsmParser PowerPCDesc PowerPCInfo PowerPCAsmPrinter NVPTXCodeGen NVPTXDesc NVPTXInfo NVPTXAsmPrinter MSP430Disassembler MSP430CodeGen MSP430AsmParser MSP430Desc MSP430Info MSP430AsmPrinter MipsDisassembler MipsCodeGen MipsAsmParser MipsDesc MipsInfo MipsAsmPrinter LanaiDisassembler LanaiCodeGen LanaiAsmParser LanaiDesc LanaiAsmPrinter LanaiInfo HexagonDisassembler HexagonCodeGen HexagonAsmParser HexagonDesc HexagonInfo BPFDisassembler BPFCodeGen BPFAsmParser BPFDesc BPFInfo BPFAsmPrinter ARMDisassembler ARMCodeGen ARMAsmParser ARMDesc ARMInfo ARMAsmPrinter ARMUtils AMDGPUDisassembler AMDGPUCodeGen AMDGPUAsmParser AMDGPUDesc AMDGPUInfo AMDGPUAsmPrinter AMDGPUUtils AArch64Disassembler AArch64CodeGen AArch64AsmParser AArch64Desc AArch64Info AArch64AsmPrinter AArch64Utils ObjectYAML
Interpreter TableGen DlltoolDriver OptRemarks XRay OrcJIT MIRParser MCA LineEditor Symbolize DebugInfoPDB DebugInfoDWARF LibDriver Option Coroutines ipo Instrumentation Vectorize Linker IRReader AsmParser Coverage X86Disassembler X86AsmParser MCJIT ExecutionEngine RuntimeDyld X86CodeGen GlobalISel X86Desc X86Info MCDisassembler X86AsmPrinter X86Utils SelectionDAG AsmPrinter CodeGen Target ScalarOpts InstCombine AggressiveInstCombine TransformUtils BitWriter Analysis ProfileData Object MCParser MC DebugInfoCodeView DebugInfoMSF BitReader Core BinaryFormat Support Demangle)


target_link_libraries(
    ${target}
    ${llvm_libs}
)
