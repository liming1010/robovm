#include <llvm-c/Core.h>
#include <llvm-c/Object.h>
#include <llvm-c/TargetMachine.h>
#include <llvm/DebugInfo/DIContext.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/MC/MCAsmBackend.h>
#include <llvm/MC/MCAsmInfo.h>
#include <llvm/MC/MCContext.h>
#include <llvm/MC/MCInstPrinter.h>
#include <llvm/MC/MCInstrInfo.h>
#include <llvm/MC/MCObjectFileInfo.h>
#include <llvm/MC/MCParser/AsmLexer.h>
#include <llvm/MC/MCRegisterInfo.h>
#include <llvm/MC/MCSectionMachO.h>
#include <llvm/MC/MCStreamer.h>
#include <llvm/MC/MCSubtargetInfo.h>
#include <llvm/MC/MCParser/MCTargetAsmParser.h>
#include <llvm/Object/ObjectFile.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Target/TargetSubtargetInfo.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/ToolOutputFile.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/DebugInfo/DWARF/DWARFContext.h>
#include <cstring>
#include <string>
#include <stdio.h>
#include <locale.h>
#ifdef __APPLE__
#include <xlocale.h>
#endif
#include <iostream>

#include "LLVMExtra.h"


using namespace llvm;
using namespace llvm::object;

DEFINE_SIMPLE_CONVERSION_FUNCTIONS(Target, LLVMTargetRef)
DEFINE_SIMPLE_CONVERSION_FUNCTIONS(TargetMachine, LLVMTargetMachineRef)
DEFINE_SIMPLE_CONVERSION_FUNCTIONS(TargetOptions, LLVMTargetOptionsRef)
DEFINE_SIMPLE_CONVERSION_FUNCTIONS(PassManagerBuilder, LLVMPassManagerBuilderRef)

inline OwningBinary<ObjectFile> *unwrap(LLVMObjectFileRef OF) {
  return reinterpret_cast<OwningBinary<ObjectFile> *>(OF);
}

const char *llvmHostTriple = LLVM_HOST_TRIPLE;

void LLVMPassManagerBuilderSetDisableTailCalls(LLVMPassManagerBuilderRef PMB,
                                            LLVMBool Value) {
  PassManagerBuilder *Builder = unwrap(PMB);
  Builder->DisableTailCalls = Value;
}

void LLVMPassManagerBuilderUseAlwaysInliner(LLVMPassManagerBuilderRef PMB, LLVMBool InsertLifetime) {
  PassManagerBuilder *Builder = unwrap(PMB);
  Builder->Inliner = createAlwaysInlinerPass(InsertLifetime);
}

LLVMBool LLVMParseIR(LLVMMemoryBufferRef MemBuf, LLVMModuleRef *OutModule, char **OutMessage) {
  return LLVMParseIRInContext(LLVMGetGlobalContext(), MemBuf, OutModule, OutMessage);
}

LLVMTargetRef LLVMLookupTarget(const char *Triple, char **ErrorMessage) {
  std::string Error;
  const Target *TheTarget = TargetRegistry::lookupTarget(std::string(Triple), Error);
  if (!TheTarget) {
    *ErrorMessage = strdup(Error.c_str());
    return NULL;
  }
  return wrap(TheTarget);
}

//Reloc::Model getRelocationModel() const;
//CodeModel::Model getCodeModel() const;
//CodeGenOpt::Level getOptLevel() const;

LLVMBool LLVMTargetMachineGetAsmVerbosityDefault(LLVMTargetMachineRef T) {
    return unwrap(T)->getAsmVerbosityDefault();
}

LLVMBool LLVMTargetMachineGetDataSections(LLVMTargetMachineRef T) {
    return unwrap(T)->getDataSections();
}

LLVMBool LLVMTargetMachineGetFunctionSections(LLVMTargetMachineRef T) {
    return unwrap(T)->getFunctionSections();
}

void LLVMTargetMachineSetDataSections(LLVMTargetMachineRef T, LLVMBool Value) {
    unwrap(T)->Options.DataSections = Value != 0;
}

void LLVMTargetMachineSetFunctionSections(LLVMTargetMachineRef T, LLVMBool Value) {
    unwrap(T)->Options.FunctionSections = Value != 0;
}

LLVMTargetOptionsRef LLVMGetTargetMachineTargetOptions(LLVMTargetMachineRef T) {
    TargetMachine *TM = unwrap(T);
    return wrap(&(TM->Options));
}

LLVMBool LLVMTargetOptionsGetPrintMachineCode(LLVMTargetOptionsRef O) { return (LLVMBool) unwrap(O)->PrintMachineCode; }
void LLVMTargetOptionsSetPrintMachineCode(LLVMTargetOptionsRef O, LLVMBool V) { unwrap(O)->PrintMachineCode = V; }

LLVMBool LLVMTargetOptionsGetLessPreciseFPMADOption(LLVMTargetOptionsRef O) { return (LLVMBool) unwrap(O)->LessPreciseFPMADOption; }
void LLVMTargetOptionsSetLessPreciseFPMADOption(LLVMTargetOptionsRef O, LLVMBool V) { unwrap(O)->LessPreciseFPMADOption = V; }

LLVMBool LLVMTargetOptionsGetUnsafeFPMath(LLVMTargetOptionsRef O) { return (LLVMBool) unwrap(O)->UnsafeFPMath; }
void LLVMTargetOptionsSetUnsafeFPMath(LLVMTargetOptionsRef O, LLVMBool V) { unwrap(O)->UnsafeFPMath = V; }

LLVMBool LLVMTargetOptionsGetNoInfsFPMath(LLVMTargetOptionsRef O) { return (LLVMBool) unwrap(O)->NoInfsFPMath; }
void LLVMTargetOptionsSetNoInfsFPMath(LLVMTargetOptionsRef O, LLVMBool V) { unwrap(O)->NoInfsFPMath = V; }

LLVMBool LLVMTargetOptionsGetNoNaNsFPMath(LLVMTargetOptionsRef O) { return (LLVMBool) unwrap(O)->NoNaNsFPMath; }
void LLVMTargetOptionsSetNoNaNsFPMath(LLVMTargetOptionsRef O, LLVMBool V) { unwrap(O)->NoNaNsFPMath = V; }

LLVMBool LLVMTargetOptionsGetHonorSignDependentRoundingFPMathOption(LLVMTargetOptionsRef O) { return (LLVMBool) unwrap(O)->HonorSignDependentRoundingFPMathOption; }
void LLVMTargetOptionsSetHonorSignDependentRoundingFPMathOption(LLVMTargetOptionsRef O, LLVMBool V) { unwrap(O)->HonorSignDependentRoundingFPMathOption = V; }

LLVMBool LLVMTargetOptionsGetNoZerosInBSS(LLVMTargetOptionsRef O) { return (LLVMBool) unwrap(O)->NoZerosInBSS; }
void LLVMTargetOptionsSetNoZerosInBSS(LLVMTargetOptionsRef O, LLVMBool V) { unwrap(O)->NoZerosInBSS = V; }

LLVMBool LLVMTargetOptionsGetGuaranteedTailCallOpt(LLVMTargetOptionsRef O) { return (LLVMBool) unwrap(O)->GuaranteedTailCallOpt; }
void LLVMTargetOptionsSetGuaranteedTailCallOpt(LLVMTargetOptionsRef O, LLVMBool V) { unwrap(O)->GuaranteedTailCallOpt = V; }

unsigned LLVMTargetOptionsGetStackAlignmentOverride(LLVMTargetOptionsRef O) { return (unsigned) unwrap(O)->StackAlignmentOverride; }
void LLVMTargetOptionsSetStackAlignmentOverride(LLVMTargetOptionsRef O, unsigned V) { unwrap(O)->StackAlignmentOverride = V; }

LLVMBool LLVMTargetOptionsGetEnableFastISel(LLVMTargetOptionsRef O) { return (LLVMBool) unwrap(O)->EnableFastISel; }
void LLVMTargetOptionsSetEnableFastISel(LLVMTargetOptionsRef O, LLVMBool V) { unwrap(O)->EnableFastISel = V; }

void LLVMModuleSetPIELevel(LLVMModuleRef M, LLVMPIELevel V) { unwrap(M)->setPIELevel((PIELevel::Level)V); }
LLVMPIELevel LLVMModuleGetPIELevel(LLVMModuleRef M) { return (LLVMPIELevel)unwrap(M)->getPIELevel();}

LLVMBool LLVMTargetOptionsGetUseInitArray(LLVMTargetOptionsRef O) { return (LLVMBool) unwrap(O)->UseInitArray; }
void LLVMTargetOptionsSetUseInitArray(LLVMTargetOptionsRef O, LLVMBool V) { unwrap(O)->UseInitArray = V; }

LLVMFloatABIType LLVMTargetOptionsGetFloatABIType(LLVMTargetOptionsRef O) { return (LLVMFloatABIType) unwrap(O)->FloatABIType; }
void LLVMTargetOptionsSetFloatABIType(LLVMTargetOptionsRef O, LLVMFloatABIType V) { unwrap(O)->FloatABIType = (FloatABI::ABIType) V; }

LLVMFPOpFusionMode LLVMTargetOptionsGetAllowFPOpFusion(LLVMTargetOptionsRef O) { return (LLVMFPOpFusionMode) unwrap(O)->AllowFPOpFusion; }
void LLVMTargetOptionsSetAllowFPOpFusion(LLVMTargetOptionsRef O, LLVMFPOpFusionMode V) { unwrap(O)->AllowFPOpFusion = (FPOpFusion::FPOpFusionMode) V; }

static void assembleDiagHandler(const SMDiagnostic &Diag, void *Context) {
  raw_string_ostream *OS = (raw_string_ostream*) Context;
  Diag.print(0, *OS, false);
}

int LLVMTargetMachineAssembleToFile(LLVMTargetMachineRef TM, LLVMMemoryBufferRef Mem, char* FilePath,
  LLVMBool RelaxAll, LLVMBool IncrementalLinkerCompatible, LLVMBool DWARFMustBeAtTheEnd,  LLVMBool NoExecStack, char **ErrorMessage) {
  *ErrorMessage = NULL;

#if !defined(WIN32)
  locale_t loc = newlocale(LC_ALL_MASK, "C", 0);
  locale_t oldLoc = uselocale(loc);
#endif

  TargetMachine *TheTargetMachine = unwrap(TM);
  const Target *TheTarget = &(TheTargetMachine->getTarget());

  std::string TripleName = TheTargetMachine->getTargetTriple().str();
  std::string MCPU = TheTargetMachine->getTargetCPU().str();
  std::string FeaturesStr = TheTargetMachine->getTargetFeatureString().str();
  Reloc::Model RelocModel = TheTargetMachine->getRelocationModel();
  CodeModel::Model CMModel = TheTargetMachine->getCodeModel();

  std::unique_ptr<MemoryBuffer> Buffer(unwrap(Mem));

  std::string DiagStr;
  raw_string_ostream DiagStream(DiagStr);
  SourceMgr SrcMgr;
  SrcMgr.setDiagHandler(assembleDiagHandler, &DiagStream);

  // Tell SrcMgr about this buffer, which is what the parser will pick up.
  SrcMgr.AddNewSourceBuffer(std::move(Buffer), SMLoc());

  // Record the location of the include directories so that the lexer can find
  // it later.
//  SrcMgr.setIncludeDirs(IncludeDirs);

  std::unique_ptr<MCRegisterInfo> MRI(TheTarget->createMCRegInfo(TripleName));
  std::unique_ptr<MCAsmInfo> MAI(TheTarget->createMCAsmInfo(*MRI, TripleName));
  std::unique_ptr<MCObjectFileInfo> MOFI(new MCObjectFileInfo());
  MCContext Ctx(MAI.get(), MRI.get(), MOFI.get(), &SrcMgr);
  MOFI->InitMCObjectFileInfo(TheTargetMachine->getTargetTriple(), RelocModel, CMModel, Ctx);

  std::unique_ptr<MCInstrInfo> MCII(TheTarget->createMCInstrInfo());
  std::unique_ptr<MCSubtargetInfo> STI(TheTarget->createMCSubtargetInfo(TripleName, MCPU, FeaturesStr));

  std::error_code EC;
  raw_fd_ostream Out(FilePath, EC, sys::fs::F_None);

  std::unique_ptr<MCStreamer> Str;
  MCCodeEmitter *CE = TheTarget->createMCCodeEmitter(*MCII, *MRI, Ctx);
  MCAsmBackend *MAB = TheTarget->createMCAsmBackend(*MRI, TripleName, MCPU);
  Str.reset(TheTarget->createMCObjectStreamer(TheTargetMachine->getTargetTriple(), Ctx, *MAB,
                                              Out, CE, *STI, RelaxAll != 0,
                                              IncrementalLinkerCompatible != 0, DWARFMustBeAtTheEnd != 0));
  if (NoExecStack != 0)
    Str->InitSections(true);

  MCTargetOptions MCOptions;
  std::unique_ptr<MCAsmParser> Parser(createMCAsmParser(SrcMgr, Ctx, *Str, *MAI));
  std::unique_ptr<MCTargetAsmParser> TAP(TheTarget->createMCAsmParser(*STI, *Parser, *MCII, MCOptions));
  if (!TAP) {
    *ErrorMessage = strdup("this target does not support assembly parsing");
    goto done;
  }

  Parser->setTargetParser(*TAP.get());

  if (Parser->Run(false)) {
    *ErrorMessage = strdup(DiagStream.str().c_str());
    goto done;
  }
  Out.flush();

done:
#if !defined(WIN32)
  uselocale(oldLoc);
  freelocale(loc);
#endif
  return *ErrorMessage ? 1 : 0;
}


/*static LLVMBool LLVMTargetMachineEmit(LLVMTargetMachineRef T, LLVMModuleRef M,
  raw_pwrite_stream &OS, LLVMCodeGenFileType codegen, char **ErrorMessage) {
  TargetMachine* TM = unwrap(T);
  Module* Mod = unwrap(M);

  llvm::legacy::PassManager pass;

  std::string error;

  const DataLayout td = TM->createDataLayout();
  Mod->setDataLayout(td);

  TargetMachine::CodeGenFileType ft;
  switch (codegen) {
    case LLVMAssemblyFile:
      ft = TargetMachine::CGFT_AssemblyFile;
      break;
    default:
      ft = TargetMachine::CGFT_ObjectFile;
      break;
  }
  if (TM->addPassesToEmitFile(pass, OS, ft)) {
    error = "TargetMachine can't emit a file of this type";
    *ErrorMessage = strdup(error.c_str());
    return true;
  }

  pass.run(*Mod);

  OS.flush();
  return false;
}

LLVMBool LLVMTargetMachineEmitToOutputStream(LLVMTargetMachineRef T, LLVMModuleRef M,
  void *JOStream, LLVMCodeGenFileType codegen, char** ErrorMessage) {

#if !defined(WIN32)
  locale_t loc = newlocale(LC_ALL_MASK, "C", 0);
  locale_t oldLoc = uselocale(loc);
#endif
  //formatted_raw_ostream Out(*((raw_java_ostream*) JOStream));
  bool Result = LLVMTargetMachineEmit(T, M, *((raw_java_ostream*) JOStream), codegen, ErrorMessage);
  ((raw_java_ostream*) JOStream)->flush();

#if !defined(WIN32)
  uselocale(oldLoc);
  freelocale(loc);
#endif

  return Result;
}
*/
void LLVMGetLineInfoForAddressRange(LLVMObjectFileRef O, uint64_t Address, uint64_t Size, int* OutSize, uint64_t** Out) {
  DWARFContextInMemory* ctx = new DWARFContextInMemory(*(unwrap(O)->getBinary()));
  DILineInfoTable lineTable = ctx->getLineInfoForAddressRange(Address, Size);
  *OutSize = lineTable.size();
  *Out = NULL;
  if (lineTable.size() > 0) {
    *Out = (uint64_t*) calloc(lineTable.size() * 2, sizeof(uint64_t));
    for (int i = 0; i < lineTable.size(); i++) {
      std::pair<uint64_t, DILineInfo> p = lineTable[i];
      (*Out)[i * 2] = p.first;
      (*Out)[i * 2 + 1] = p.second.Line;
    }
  }
}

size_t LLVMCopySectionContents(LLVMSectionIteratorRef SI, char* Dest, size_t DestSize) {
  uint64_t SectionSize = LLVMGetSectionSize(SI);
  size_t Size = SectionSize > DestSize ? DestSize : SectionSize;
  const char* Contents = LLVMGetSectionContents(SI);
  memcpy(Dest, Contents, (size_t) Size);
  return Size;
}
