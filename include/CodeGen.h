#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Scalar/Reassociate.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>
#include <memory>
#include <map>
#include <string>

extern std::unique_ptr<llvm::LLVMContext> TheContext;
extern std::unique_ptr<llvm::IRBuilder<>> Builder;
extern std::unique_ptr<llvm::Module> TheModule;
extern std::map<std::string, llvm::Value *> NamedValues;
extern std::unique_ptr<llvm::FunctionPassManager> FPM;
extern std::unique_ptr<llvm::LoopAnalysisManager> LAM;
extern std::unique_ptr<llvm::FunctionAnalysisManager> FAM;
extern std::unique_ptr<llvm::CGSCCAnalysisManager> CGAM;
extern std::unique_ptr<llvm::ModuleAnalysisManager> MAM;

void InitializeModule();
