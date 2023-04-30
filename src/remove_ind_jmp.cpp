//==============================================================================
// FILE:
//    RemoveIndirectJumps.cpp
//
// DESCRIPTION:
//    Counts the number of static function calls in the input module. `Static`
//    refers to the fact that the analysed functions calls are compile-time
//    calls (as opposed to `dynamic`, i.e. run-time). Only direct function
//    calls are considered. Calls via functions pointers are not taken into
//    account.
//
//    This pass is used in `static`, a tool implemented in tools/StaticMain.cpp
//    that is a wrapper around RemoveIndirectJumps. `static` allows you to run
//    RemoveIndirectJumps without `opt`.
//
// USAGE:
//    1. Legacy PM
//      opt -load libRemoveIndirectJumps.dylib -legacy-static-cc `\`
//        -analyze <input-llvm-file>
//    2. New PM
//      opt -load-pass-plugin libRemoveIndirectJumps.dylib `\`
//        -passes="print<static-cc>" `\`
//        -disable-output <input-llvm-file>
//
// License: MIT
//==============================================================================
#include "remove_ind_jmp.h"

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;

// Pretty-prints the result of this analysis
static void printIndJumps(llvm::raw_ostream &OutS, const ResultIndJumps &indJumps);

//------------------------------------------------------------------------------
// RemoveIndirectJumps Implementation
//------------------------------------------------------------------------------
RemoveIndirectJumps::Result RemoveIndirectJumps::runOnModule(Module &M) {
    std::map<const llvm::Function *, llvm::Instruction*> Res;

    for (auto &Func : M) {
        for (auto &BB : Func) {
            for (auto &Ins : BB) {
                if (Ins.getOpcode() == Instruction::IndirectBr) {
                    Res.insert(std::make_pair(&Func, &Ins));
                }
            }
        }
    }

    return Res;
}

PreservedAnalyses
RemoveIndirectJumpsPrinter::run(Module &M, ModuleAnalysisManager &MAM) {

    auto IndirectJumps = MAM.getResult<RemoveIndirectJumps>(M);

    printIndJumps(OS, IndirectJumps);
    return PreservedAnalyses::none();
}

RemoveIndirectJumps::Result
RemoveIndirectJumps::run(llvm::Module &M, llvm::ModuleAnalysisManager &) {
    return runOnModule(M);
}


//------------------------------------------------------------------------------
// New PM Registration
//------------------------------------------------------------------------------
AnalysisKey RemoveIndirectJumps::Key;

llvm::PassPluginLibraryInfo getRemoveIndirectJumpsPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "static-cc", LLVM_VERSION_STRING,
                    [](PassBuilder &PB) {
                        // #1 REGISTRATION FOR "opt -passes=print<static-cc>"
                        PB.registerPipelineParsingCallback(
                                [&](StringRef Name, ModulePassManager &MPM,
                                        ArrayRef<PassBuilder::PipelineElement>) {
                                    if (Name == "remove-jmps") {
                                        MPM.addPass(RemoveIndirectJumpsPrinter(llvm::errs()));
                                        return true;
                                    }
                                    return false;
                                });
                        // #2 REGISTRATION FOR "MAM.getResult<RemoveIndirectJumps>(Module)"
                        PB.registerAnalysisRegistrationCallback(
                                [](ModuleAnalysisManager &MAM) {
                                    MAM.registerPass([&] { return RemoveIndirectJumps(); });
                                });
                    }};
};

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return getRemoveIndirectJumpsPluginInfo();
}

//------------------------------------------------------------------------------
// Helper functions
//------------------------------------------------------------------------------
static void printIndJumps(raw_ostream &OutS, const ResultIndJumps &indJumps) {
    OutS << "=================================================\n";
    OutS << "LLVM-TUTOR: static analysis results\n";
    OutS << "=================================================\n";

    for (auto &jmpSite : indJumps) {
        OutS << format("%s:\n", jmpSite.first->getName().str().c_str());
        OutS << format("%s %s\n", jmpSite.second->getOpcodeName(), jmpSite.second->getOperand(0));
    }

    // for (auto &callSite : NextInsnMap) {
    //     const Function* F = callSite.first;

    //     SmallVector<BasicBlock*> destBlocks;
    //     for (auto insn : *callSite.second) {
    //         destBlocks.append(1, (insn->getParent()->splitBasicBlock(insn)));
    //     }

    //     Instruction* retToRemove;
    //     for (auto &BB : *F) {
    //         for (auto &I : BB) {
    //         // for (auto Inst = BB.begin(), IE = BB.end(); Inst != IE; ++Inst) {
    //             // Instruction I = *Inst;
    //             if (I.getOpcode() == Instruction::Ret) {
    //                 OutS<<"HERE\n";
    //                 SmallVector<BasicBlock*> jumpBlocks;
    //                 // for (auto jumpBlock : jumpBlocks) {
    //                 //     jumpBlock = BasicBlock::Create(F->getContext());
    //                 // }
    //                 BasicBlock* failBlock = BasicBlock::Create(F->getContext());
    //                 IRBuilder<> failBuilder(failBlock);
    //                     // IRBuilder<> ifElseBuilder(const_cast<Instruction*>(&I));

    //                 for (auto insn : *callSite.second) {
    //                     OutS<<"HERE1\n";
    //                     BasicBlock* jumpBlock = BasicBlock::Create(F->getContext());
    //                     IRBuilder<> jumpBuilder(F->getContext());
    //                     BasicBlock* destBlock = insn->getParent()->splitBasicBlock(insn);
    //                     Instruction* jInst = jumpBuilder.CreateCallBr(insn->getParent()->getParent(), destBlock, nullptr);

    //                     jInst->insertBefore(const_cast<Instruction*>(&I));
    //                     const_cast<Instruction*>(&I)->eraseFromParent();
    //                     // jInst->insertBefore(&I);
    //                     break;
    //                 }
    //                 break;
    //                 //  BranchInst* ifInst = builder.CreateCondBr()
                    
    //             }
    //         }
    //     }
    // }

    OutS << "-------------------------------------------------" << "\n";
}