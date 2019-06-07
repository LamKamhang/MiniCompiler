# +----------------------------------------------+
# | project settings                             |
# +--------------------------------------------- +
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(ncc)
set(target_name ncc)

set(source_files
	src/main.cxx
	src/parser/scanner.cpp
	src/parser/parser.cpp
	src/ast/ast.cc
	src/lib/json/jsoncpp.cc
	src/ir/ir.cc
	src/util/json.cc
	src/tc/tc.cc
)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/ir)
include_directories(${PROJECT_SOURCE_DIR}/src/lib)
include_directories(${PROJECT_SOURCE_DIR}/src/util)
include_directories(${PROJECT_SOURCE_DIR}/src/ast)
include_directories(${PROJECT_SOURCE_DIR}/src/tc)

# +----------------------------------------------+
# | Complilation flags                           |
# +--------------------------------------------- +
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
if (PROFILE_FLAG)
	message(STATUS "profiling")
	if (CMAKE_COMPILER_IS_GNUCXX)
		# set(CMAKE_EXE_LINKER_FLAGS "-pthread")
		set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g -pg")
		set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -pg")
	endif (CMAKE_COMPILER_IS_GNUCXX)
else (PROFILE_FLAG)
	if (CMAKE_COMPILER_IS_GNUCXX)
		# set(CMAKE_EXE_LINKER_FLAGS "-pthread")
		set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g")
		set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")
	endif (CMAKE_COMPILER_IS_GNUCXX)
endif (PROFILE_FLAG)

# +----------------------------------------------+
# | Third Party Libray                           |
# +--------------------------------------------- +
find_package(
    LLVM
    REQUIRED
    CONFIG
)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

if (${LLVM_PACKAGE_VERSION} VERSION_LESS "7.0")
	llvm_map_components_to_libnames(llvm_libs
		LTO Passes ObjCARCOpts WindowsManifest FuzzMutate XCoreDisassembler XCoreCodeGen XCoreDesc XCoreInfo XCoreAsmPrinter SystemZDisassembler SystemZCodeGen SystemZAsmParser SystemZDesc SystemZInfo SystemZAsmPrinter SparcDisassembler SparcCodeGen SparcAsmParser SparcDesc SparcInfo SparcAsmPrinter PowerPCDisassembler PowerPCCodeGen PowerPCAsmParser PowerPCDesc PowerPCInfo PowerPCAsmPrinter NVPTXCodeGen NVPTXDesc NVPTXInfo NVPTXAsmPrinter MSP430CodeGen MSP430Desc MSP430Info MSP430AsmPrinter MipsDisassembler MipsCodeGen MipsAsmParser MipsDesc MipsInfo MipsAsmPrinter LanaiDisassembler LanaiCodeGen LanaiAsmParser LanaiDesc LanaiAsmPrinter LanaiInfo HexagonDisassembler HexagonCodeGen HexagonAsmParser HexagonDesc HexagonInfo BPFDisassembler BPFCodeGen BPFAsmParser BPFDesc BPFInfo BPFAsmPrinter ARMDisassembler ARMCodeGen ARMAsmParser ARMDesc ARMInfo ARMAsmPrinter ARMUtils AMDGPUDisassembler AMDGPUCodeGen AMDGPUAsmParser AMDGPUDesc AMDGPUInfo AMDGPUAsmPrinter AMDGPUUtils AArch64Disassembler AArch64CodeGen AArch64AsmParser AArch64Desc AArch64Info AArch64AsmPrinter AArch64Utils ObjectYAML
		Interpreter TableGen DlltoolDriver XRay OrcJIT MIRParser Symbolize DebugInfoPDB DebugInfoDWARF LibDriver Option Coroutines ipo Instrumentation Vectorize Linker IRReader AsmParser Coverage X86Disassembler X86AsmParser MCJIT ExecutionEngine RuntimeDyld X86CodeGen GlobalISel X86Desc X86Info MCDisassembler X86AsmPrinter X86Utils SelectionDAG AsmPrinter CodeGen Target ScalarOpts InstCombine TransformUtils BitWriter Analysis ProfileData Object MCParser MC DebugInfoCodeView DebugInfoMSF BitReader Core BinaryFormat Support Demangle
	)
else ()
	llvm_map_components_to_libnames(llvm_libs
		LTO Passes ObjCARCOpts WindowsManifest FuzzMutate TextAPI XCoreDisassembler XCoreCodeGen XCoreDesc XCoreInfo XCoreAsmPrinter WebAssemblyDisassembler WebAssemblyCodeGen WebAssemblyDesc WebAssemblyAsmPrinter WebAssemblyAsmParser WebAssemblyInfo SystemZDisassembler SystemZCodeGen SystemZAsmParser SystemZDesc SystemZInfo SystemZAsmPrinter SparcDisassembler SparcCodeGen SparcAsmParser SparcDesc SparcInfo SparcAsmPrinter PowerPCDisassembler PowerPCCodeGen PowerPCAsmParser PowerPCDesc PowerPCInfo PowerPCAsmPrinter NVPTXCodeGen NVPTXDesc NVPTXInfo NVPTXAsmPrinter MSP430Disassembler MSP430CodeGen MSP430AsmParser MSP430Desc MSP430Info MSP430AsmPrinter MipsDisassembler MipsCodeGen MipsAsmParser MipsDesc MipsInfo MipsAsmPrinter LanaiDisassembler LanaiCodeGen LanaiAsmParser LanaiDesc LanaiAsmPrinter LanaiInfo HexagonDisassembler HexagonCodeGen HexagonAsmParser HexagonDesc HexagonInfo BPFDisassembler BPFCodeGen BPFAsmParser BPFDesc BPFInfo BPFAsmPrinter ARMDisassembler ARMCodeGen ARMAsmParser ARMDesc ARMInfo ARMAsmPrinter ARMUtils AMDGPUDisassembler AMDGPUCodeGen AMDGPUAsmParser AMDGPUDesc AMDGPUInfo AMDGPUAsmPrinter AMDGPUUtils AArch64Disassembler AArch64CodeGen AArch64AsmParser AArch64Desc AArch64Info AArch64AsmPrinter AArch64Utils ObjectYAML
		Interpreter TableGen DlltoolDriver OptRemarks XRay OrcJIT MIRParser MCA LineEditor Symbolize DebugInfoPDB DebugInfoDWARF LibDriver Option Coroutines ipo Instrumentation Vectorize Linker IRReader AsmParser Coverage X86Disassembler X86AsmParser MCJIT ExecutionEngine RuntimeDyld X86CodeGen GlobalISel X86Desc X86Info MCDisassembler X86AsmPrinter X86Utils SelectionDAG AsmPrinter CodeGen Target ScalarOpts InstCombine AggressiveInstCombine TransformUtils BitWriter Analysis ProfileData Object MCParser MC DebugInfoCodeView DebugInfoMSF BitReader Core BinaryFormat Support Demangle
	)
endif()
# +----------------------------------------------+
# | Build executable                             |
# +--------------------------------------------- +
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(${target_name}
   ${source_files}
)

target_link_libraries(
    ${target_name}
    ${llvm_libs}
)