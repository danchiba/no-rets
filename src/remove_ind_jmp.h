//========================================================================
// FILE:
//    RemoveIndirectJumps.h
//
// DESCRIPTION:
//    Declares the RemoveIndirectJumps Passes
//      * new pass manager interface
//      * legacy pass manager interface
//      * printer pass for the new pass manager
//
// License: MIT
//========================================================================
#ifndef LLVM_TUTOR_RemoveIndirectJumps_H
#define LLVM_TUTOR_RemoveIndirectJumps_H

#include <map>
#include "llvm/ADT/SetVector.h"
#include "llvm/IR/AbstractCallSite.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

//------------------------------------------------------------------------------
// New PM interface
//------------------------------------------------------------------------------
using ResultIndJumps = std::map<const llvm::Function *, llvm::Instruction*>;

struct RemoveIndirectJumps : public llvm::AnalysisInfoMixin<RemoveIndirectJumps> {
  using Result = ResultIndJumps;
  Result run(llvm::Module &M, llvm::ModuleAnalysisManager &);
  Result runOnModule(llvm::Module &M);
  // Part of the official API:
  //  https://llvm.org/docs/WritingAnLLVMNewPMPass.html#required-passes
  static bool isRequired() { return true; }

private:
  // A special type used by analysis passes to provide an address that
  // identifies that particular analysis pass type.
  static llvm::AnalysisKey Key;
  friend struct llvm::AnalysisInfoMixin<RemoveIndirectJumps>;
};

//------------------------------------------------------------------------------
// New PM interface for the printer pass
//------------------------------------------------------------------------------
class RemoveIndirectJumpsPrinter
    : public llvm::PassInfoMixin<RemoveIndirectJumpsPrinter> {
public:
  explicit RemoveIndirectJumpsPrinter(llvm::raw_ostream &OutS) : OS(OutS) {}
  llvm::PreservedAnalyses run(llvm::Module &M,
                              llvm::ModuleAnalysisManager &MAM);
  // Part of the official API:
  //  https://llvm.org/docs/WritingAnLLVMNewPMPass.html#required-passes
  static bool isRequired() { return true; }

private:
  llvm::raw_ostream &OS;
};

#endif // LLVM_TUTOR_RemoveIndirectJumps_H