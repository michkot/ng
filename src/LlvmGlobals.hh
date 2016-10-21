#pragma once

namespace llvm {
  class Module;
  class DataLayout;
  class LLVMContext;
}

extern thread_local const llvm::Module* llvmModule;
extern thread_local       llvm::DataLayout llvmDataLayout;
extern thread_local const llvm::LLVMContext* llvmContext;