//==============================================================================
// FILE:
//    StaticCallCounter.cpp
//
// DESCRIPTION:
//    Counts the number of static function calls in the input module. `Static`
//    refers to the fact that the analysed functions calls are compile-time
//    calls (as opposed to `dynamic`, i.e. run-time). Only direct function
//    calls are considered. Calls via functions pointers are not taken into
//    account.
//
//    This pass is used in `static`, a tool implemented in tools/StaticMain.cpp
//    that is a wrapper around StaticCallCounter. `static` allows you to run
//    StaticCallCounter without `opt`.
//
// USAGE:
//    1. Legacy PM
//      opt -load libStaticCallCounter.dylib -legacy-static-cc `\`
//        -analyze <input-llvm-file>
//    2. New PM
//      opt -load-pass-plugin libStaticCallCounter.dylib `\`
//        -passes="print<static-cc>" `\`
//        -disable-output <input-llvm-file>
//
// License: MIT
//==============================================================================
#include "remove_ret.h"

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CFG.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;

// Pretty-prints the result of this analysis
static void printStaticCCResult(llvm::raw_ostream &OutS, const ResultStaticCC &DirectCalls);

//------------------------------------------------------------------------------
// StaticCallCounter Implementation
//------------------------------------------------------------------------------
StaticCallCounter::Result StaticCallCounter::runOnModule(Module &M) {
    std::map<const llvm::Function *, llvm::SetVector<Instruction*> *> Res;

    for (auto &Func : M) {
        // BasicBlock& entryBlock = Func.front();
        // errs() << format("%s: %d\n", Func.getName(), 1);
        // for (BasicBlock* callingBlock : predecessors(&entryBlock)) {

        //     errs() << callingBlock->getParent()->getName() << "\n";
        //     errs() << callingBlock->front().getOpcodeName() << "\n";
        // }
        
        for (auto &BB : Func) {
            for (auto &Ins : BB) {

                // If this is a call instruction then CB will be not null.
                auto *CB = dyn_cast<CallBase>(&Ins);
                if (nullptr == CB) {
                    continue;
                }

                // If CB is a direct function call then DirectInvoc will be not null.
                auto DirectInvoc = CB->getCalledFunction();
                if (nullptr == DirectInvoc) {
                    continue;
                }

                // We have a direct function call - update the count for the function
                // being called.
                auto resIt = Res.find(DirectInvoc);
                if (Res.end() == resIt) {
                    resIt = Res.insert(std::make_pair(DirectInvoc, new SetVector<Instruction*>())).first;
                }

                auto nextInsns = resIt->second;
                nextInsns->insert(Ins.getNextNode());
            }
        }
    }

    return Res;
}

PreservedAnalyses
StaticCallCounterPrinter::run(Module &M, ModuleAnalysisManager &MAM) {

    auto DirectCalls = MAM.getResult<StaticCallCounter>(M);

    printStaticCCResult(OS, DirectCalls);
    return PreservedAnalyses::none();
}

StaticCallCounter::Result
StaticCallCounter::run(llvm::Module &M, llvm::ModuleAnalysisManager &) {
    return runOnModule(M);
}


//------------------------------------------------------------------------------
// New PM Registration
//------------------------------------------------------------------------------
AnalysisKey StaticCallCounter::Key;

llvm::PassPluginLibraryInfo getStaticCallCounterPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "static-cc", LLVM_VERSION_STRING,
                    [](PassBuilder &PB) {
                        // #1 REGISTRATION FOR "opt -passes=print<static-cc>"
                        PB.registerPipelineParsingCallback(
                                [&](StringRef Name, ModulePassManager &MPM,
                                        ArrayRef<PassBuilder::PipelineElement>) {
                                    if (Name == "remove-rets") {
                                        MPM.addPass(StaticCallCounterPrinter(llvm::errs()));
                                        return true;
                                    }
                                    return false;
                                });
                        // #2 REGISTRATION FOR "MAM.getResult<StaticCallCounter>(Module)"
                        PB.registerAnalysisRegistrationCallback(
                                [](ModuleAnalysisManager &MAM) {
                                    MAM.registerPass([&] { return StaticCallCounter(); });
                                });
                    }};
};

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return getStaticCallCounterPluginInfo();
}

//------------------------------------------------------------------------------
// Helper functions
//------------------------------------------------------------------------------
static void printStaticCCResult(raw_ostream &OutS, const ResultStaticCC &NextInsnMap) {
    // for (auto &callSite : NextInsnMap) {
    //     OutS << format("%s:\n", callSite.first->getName().str().c_str());
    //     for (auto insn : *callSite.second) {
    //         OutS << format("%s\n", insn->getOpcodeName());
    //     }
    // }

    for (auto &callSite : NextInsnMap) {
        const Function* F = callSite.first;
        OutS << F->getName() << ":";
        // BasicBlock& entryBlock = const_cast<BasicBlock&>(F->getEntryBlock());
        // for (BasicBlock* callingBlock : predecessors(&entryBlock)) {

        //     OutS << callingBlock->getParent()->getName() << "\n";
        //     OutS << callingBlock->front().getOpcodeName() << "\n";
        // }



        // SmallVector<BasicBlock*> destBlocks;
        for (auto insn : *callSite.second) {
            OutS << insn->getParent()->getParent()->getName() << ",";
        }
        OutS << "\n";
        // Instruction* retToRemove;
        // for (auto &BB : *F) {

        //     for (auto &I : BB) {
        //     // for (auto Inst = BB.begin(), IE = BB.end(); Inst != IE; ++Inst) {
        //         // Instruction I = *Inst;
        //         if (I.getOpcode() == Instruction::Ret) {
        //             OutS << format("%d successors\n", I.getNumSuccessors());

        //             for (int i = 0; i < I.getNumSuccessors(); i++) {
        //                 OutS << I.getSuccessor(i)->getParent()->getName() << "\n";
        //                 OutS << I.getSuccessor(i)->front().getOpcodeName() << "\n";
        //             }
                    
        //         }
        //     }
        // }
    }
}
