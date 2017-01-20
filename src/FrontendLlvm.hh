#pragma once

#include "Definitions.hh"
#include "IOperation.hh"
#include "Values.hh"

#include <map>

class LlvmCfgNode;

namespace llvm
{
  class BasicBlock;
  class Module;
  class Instruction;
  class Value;
  class Type;
}

class LlvmCfgParser {
  IOperationFactory& opFactory;
  IValueContainer& vc;

  map<const llvm::BasicBlock*, LlvmCfgNode*> copyMapping;
  queue<tuple<const llvm::BasicBlock*, LlvmCfgNode*, unsigned int>> parseAndLinkTogether;

public:
  LlvmCfgParser(IOperationFactory& opFactory, IValueContainer& vc) : opFactory{opFactory},vc{vc} {}

private:
  IOperation& GetOperationFor(const llvm::Instruction& instruction) const;

  static Type GetValueType(llvm::Type* type);

  static FrontendValueId GetValueId(uint64_t id);
  static FrontendValueId GetValueId(const llvm::Value* value);
  static FrontendValueId GetValueId(const llvm::Value& value);

  static OperArg ToOperArg(const llvm::Value* value);
  static OperArg ToOperArg(const llvm::Value& value);
  static OperArg GetEmptyOperArg();
  static OperArg GetFlagsOperArg(CmpFlags flags);
  static OperArg GetFlagsOperArg(ArithFlags flags);

  static vector<OperArg> GetOperArgsForInstr(const llvm::Instruction& instr);

  bool TryGetMappedCfgNode(const llvm::BasicBlock* bb, LlvmCfgNode** outNode);

  void LinkWithOrPlanProcessing(
    LlvmCfgNode* currentNode,
    const llvm::BasicBlock* nextBlock,
    unsigned targetIndex
  );

  LlvmCfgNode& ParseBasicBlock(const llvm::BasicBlock* entryBlock);

  void DealWithConstants();

public:
  ICfgNode& ParseModule(const llvm::Module& module);

  uptr<llvm::Module> OpenIrFile(string fileName);

  ICfgNode& ParseAndOpenIrFile(string fileName);
};
