#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/DataLayout.h>

thread_local const llvm::Module* llvmModule = nullptr;
thread_local       llvm::DataLayout llvmDataLayout{"e-m:w-i64:64-f80:128-n8:16:32:64-S128"};
thread_local const llvm::LLVMContext* llvmContext = nullptr;

void setLlvmGlobalVars(llvm::Module* module)
{
  llvmModule = module;
  llvmDataLayout = module->getDataLayout();
  llvmContext = &module->getContext();
}