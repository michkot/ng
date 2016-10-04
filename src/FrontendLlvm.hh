#pragma once

#include "Definitions.hh"
#include "IOperation.hh"

#include <map>

class LlvmCfgNode;

namespace llvm 
{
  class BasicBlock;
  class Module;
  class Instruction;
  class Type;
}

class LlvmCfgParser {
  IOperationFactory& opFactory;

  map<const llvm::BasicBlock*, LlvmCfgNode*> copyMapping;
  queue<tuple<const llvm::BasicBlock*, LlvmCfgNode*, unsigned int>> parseAndLinkTogether;

public:
  LlvmCfgParser(IOperationFactory& opFactory) : opFactory{opFactory} {}

private:
  IOperation& GetOperationFor(const llvm::Instruction& instruction) const;

  static ValueType GetValueType(const llvm::Type*);

  static ValueId GetValueId(int id);
  static ValueId GetValueId(uint64_t id);
  static ValueId GetValueId(const llvm::Instruction* instr);
  static ValueId GetValueId(const llvm::Instruction& instr);

  static vector<InstrArg> GetInstrArgsFor(const llvm::Instruction& instr);

  bool TryGetMappedCfgNode(const llvm::BasicBlock* bb, LlvmCfgNode** outNode);

  void LinkWithOrPlanProcessing(
    LlvmCfgNode* currentNode,
    const llvm::BasicBlock* nextBlock,
    unsigned targetIndex
  );

  LlvmCfgNode& ParseBasicBlock(const llvm::BasicBlock* entryBlock);

public:
  ICfgNode& ParseModule(const llvm::Module& module);

  uptr<llvm::Module> OpenIrFile(string fileName);

  ICfgNode& ParseAndOpenIrFile(string fileName);
};
