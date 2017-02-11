#pragma once

#include "Definitions.hh"
#include "IOperation.hh"
#include "Values.hh"

#include "FrontedValueMapper.hh"

#include <map>
#include <boost/utility/string_view.hpp>

class LlvmCfgNode;

namespace llvm
{
  class BasicBlock;
  class Module;
  class Instruction;
  class Value;
  class Type;
  class Constant;
}

class LlvmCfgParser {
  IOperationFactory& opFactory;
  IValueContainer& vc;
  Mapper& mapper;

  map<const llvm::BasicBlock*, LlvmCfgNode*> copyMapping;
  queue<tuple<const llvm::BasicBlock*, LlvmCfgNode*, unsigned int>> parseAndLinkTogether;

public:
  LlvmCfgParser(IOperationFactory& opFactory, IValueContainer& vc, Mapper& mapper) : opFactory{opFactory}, vc{vc}, mapper{mapper} {}

private:
  IOperation& GetOperationFor(const llvm::Instruction& instruction) const;

  //non null!!
  std::vector<const llvm::Constant*> constantValuesToBeCreated;
  void constantValuesToBeCreatedInsert(const llvm::Constant* c);

  static Type GetValueType(llvm::Type* type);

  static FrontendValueId GetValueId(uint64_t id);
  static FrontendValueId GetValueId(const llvm::Value* value);
  static FrontendValueId GetValueId(const llvm::Value& value);

  static OperArg ToOperArg(const llvm::Value* value);
  static OperArg ToOperArg(const llvm::Value& value);
  static OperArg GetEmptyOperArg();
  static OperArg GetFlagsOperArg(CmpFlags flags);
  static OperArg GetFlagsOperArg(ArithFlags flags);
  static OperArg GetFlagsOperArg(CastOpsEnum kind, ArithFlags flags);

  vector<OperArg> GetOperArgsForInstr(const llvm::Instruction& instr);

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

  ICfgNode& ParseAndOpenIrFile(boost::string_view fileName);

};
