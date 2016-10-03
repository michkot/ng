#include "FrontendLlvm.hh"

#include "ICfgNode.hh"

#ifndef _MSC_VER

# ifdef __cplusplus
#  if !defined(__STDC_LIMIT_MACROS)
#   define __STDC_LIMIT_MACROS
#   define __STDC_LIMIT_MACROS_DEFINED_BY_CLANG
#  endif
#  if !defined(__STDC_CONSTANT_MACROS)
#   define __STDC_CONSTANT_MACROS
#   define __STDC_CONSTANT_MACROS_DEFINED_BY_CLANG
#  endif
# endif

#endif
#include <llvm/IR/Module.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
//#include <llvm/Support/raw_ostream.h>

//std::string dbgstr;
//llvm::raw_string_ostream dbgstr_rso(dbgstr);


class LlvmCfgNode : public CfgNode {
private:
  const llvm::Instruction& innerInstruction;
  //const vector<InstrArg> 

  /*ctr*/ LlvmCfgNode(IOperation& op, vector<InstrArg> args,
    const llvm::Instruction& inner,
    ICfgNode& prev,
    ICfgNode& next
  ) :
    innerInstruction{inner},
    CfgNode(op, prev, next) {
  }

public:

  virtual void GetDebugInfo() const override { throw NotImplementedException{}; }
  virtual vector<InstrArg> GetArguments() const override { return vector<InstrArg>(); }

  static LlvmCfgNode& CreateNode(IOperation& op, vector<InstrArg> args, const llvm::Instruction& inner)
  {
    LlvmCfgNode* newNode = new LlvmCfgNode{op, args, inner, *new StartCfgNode{}, *new TerminalCfgNode{}};
    ((ICfgNode&)newNode->prevs[0]).next = newNode;
    newNode->next->prevs.push_back(*newNode);
    return *newNode;
  }

  //beware - adding a node after terminal node here(after inproper cast) would not raise exception
  //same applies for similar linking manipulation

  LlvmCfgNode& InsertNewAfter(IOperation& op, vector<InstrArg> args, const llvm::Instruction& inner)
  {
    LlvmCfgNode* newNode = new LlvmCfgNode{op, args, inner, *this, *this->next};
    this->next = newNode;
    return *newNode;
  }

  LlvmCfgNode& InsertNewBranchAfter(IOperation& op, vector<InstrArg> args, const llvm::Instruction& inner)
  {
    LlvmCfgNode* newNode = new LlvmCfgNode{op, args, inner, *this, *this->next};
    newNode->nextFalse = new TerminalCfgNode{};
    this->next = newNode;
    return *newNode;
  }

  static void LinkTogetherTrue(ICfgNode& prev, ICfgNode& next)
  {
    if (prev.next->next != nullptr)
      throw runtime_error("prev Node has a succesor other then terminal node");
    prev.next = &next;
    next.prevs.push_back(next);
  }

  static void LinkTogetherFalse(ICfgNode& prev, ICfgNode& next)
  {
    if (prev.nextFalse->next != nullptr)
      throw runtime_error("prev Node has a succesor other then terminal node");
    prev.nextFalse = &next;
    next.prevs.push_back(next);
  }

  static void LinkTogether(ICfgNode& prev, ICfgNode& next)
  {
    LinkTogetherTrue(prev, next);
  }

  //target index specifies target when multi-target node is concenrned
  //0 -> true
  //1 -> false
  //as per LLVM's getSuccessor()
  static void LinkTogether(ICfgNode& prev, ICfgNode& next, unsigned int targetIndex)
  {
    if (targetIndex == 0)
      LinkTogetherTrue(prev, next);
    else if (targetIndex == 1)
      LinkTogetherFalse(prev, next);
    else
      throw out_of_range("unsupported targetIndex value");
  }
};

IOperation& LlvmCfgParser::GetOperationFor(const llvm::Instruction& instruction) const
{
  // Create correct operation
  IOperation* op;
  switch (instruction.getOpcode())
  {
    // Terminator instructions
  case llvm::Instruction::Ret:
    op = &opFactory.Ret();
    break;
  case llvm::Instruction::Br:
    op = &opFactory.Br();
    break;

    // Binary instructions
  case llvm::Instruction::Add:
    op = &opFactory.Add();
    break;
  case llvm::Instruction::Sub:
    op = &opFactory.Sub();
    break;
  case llvm::Instruction::Mul:
    op = &opFactory.Mul();
    break;
  case llvm::Instruction::UDiv:
    op = &opFactory.UDiv();
    break;
  case llvm::Instruction::SDiv:
    op = &opFactory.SDiv();
    break;
  case llvm::Instruction::URem:
    op = &opFactory.URem();
    break;
  case llvm::Instruction::SRem:
    op = &opFactory.SRem();
    break;

    // Bitwise binary instructions
  case llvm::Instruction::Shl:
    op = &opFactory.Shl();
    break;
  case llvm::Instruction::LShr:
    op = &opFactory.LShr();
    break;
  case llvm::Instruction::AShr:
    op = &opFactory.AShr();
    break;
  case llvm::Instruction::And:
    op = &opFactory.And();
    break;
  case llvm::Instruction::Or:
    op = &opFactory.Or();
    break;
  case llvm::Instruction::Xor:
    op = &opFactory.Xor();
    break;

    // Memory access operations
  case llvm::Instruction::Alloca:
    op = &opFactory.Alloca();
    break;
  case llvm::Instruction::Load:
    op = &opFactory.Load();
    break;
  case llvm::Instruction::Store:
    op = &opFactory.Store();
    break;
  case llvm::Instruction::GetElementPtr:
    op = &opFactory.GetElementPtr();
    break;

    // Other operations
  case llvm::Instruction::ICmp:
  {
    auto pred = static_cast<const llvm::CmpInst&>(instruction).getPredicate();
    op = &opFactory.ICmp(pred);
    break;
  }

  default:
    op = &opFactory.NotSupportedInstr();
  }

  return *op;
}

ValueType LlvmCfgParser::GetValueType(const llvm::Type&)
{
  return ValueType{};
}

ValueId LlvmCfgParser::GetValueId(int id)
{
  return ValueId{static_cast<uint64_t>(id)};
}

ValueId LlvmCfgParser::GetValueId(uint64_t id)
{
  return ValueId{id};
}

ValueId LlvmCfgParser::GetValueId(const llvm::Instruction* instr)
{
  return ValueId{reinterpret_cast<uintptr_t>(instr)};
}

vector<InstrArg> LlvmCfgParser::GetInstrArgsFor(const llvm::Instruction& instr)
{
  vector<InstrArg> args;

  //instr.print()
  //instr.print(llvm::errs()/*, true*/);
  //llvm::errs() << "\n";

  unsigned num = instr.getNumOperands();
  switch (instr.getOpcode())
  {
    //case llvm::Instruction::Ret:
    //  break;
    //case llvm::Instruction::Br:
    //  break;
  case llvm::Instruction::Call:
  {
    //auto& typedInstr = static_cast<const llvm::DbgDeclareInst&>(instr);
    //typedInstr.getVariable().

    auto& typedInstr = static_cast<const llvm::CallInst&>(instr);
    auto& x = *typedInstr.getOperand(0);
    auto func = typedInstr.getCalledFunction();
    if (func != nullptr)
    {

    }
    else
    {
      auto val = typedInstr.getCalledValue();
    }
    break;
  }
  default:
  {
    args.emplace_back(
      GetValueId(&instr),
      GetValueType(*instr.getType())
    );
    for (int i = 0; i < num; ++i)
    {
      const auto& operand = *instr.getOperand(i);
      args.emplace_back(
        GetValueId(0),
        GetValueType(*operand.getType())
      );

    }
    break;
  }
  }

  return args;
}

bool LlvmCfgParser::TryGetMappedCfgNode(const llvm::BasicBlock* bb, LlvmCfgNode** outNode)
{
  auto assignedNodeIt = copyMapping.find(bb);
  if (assignedNodeIt != copyMapping.end())
  {
    *outNode = (*assignedNodeIt).second;
    return true;
  }
  return false;
}

void LlvmCfgParser::LinkWithOrPlanProcessing(
    LlvmCfgNode* currentNode, 
    const llvm::BasicBlock* nextBlock, 
    unsigned targetIndex
    )
{
  LlvmCfgNode* nextBlockNode;
  if (TryGetMappedCfgNode(nextBlock, &nextBlockNode))
  { //this block was already parsed before            
    //just link it together
    LlvmCfgNode::LinkTogether(*currentNode, *nextBlockNode, targetIndex);
  }
  else
  { //we need to plan parsing of this block and link it when done
    parseAndLinkTogether.emplace(nextBlock, currentNode, targetIndex);
  }
}

LlvmCfgNode& LlvmCfgParser::ParseBasicBlock(const llvm::BasicBlock* entryBlock)
{
  LlvmCfgNode* currentNode{nullptr};
  //if the BB was already parsed
  if (TryGetMappedCfgNode(entryBlock, &currentNode))
    return *currentNode;

  //first BB instruction
  auto instrPtr = &entryBlock->front();

  //get matching op 
  auto& op = GetOperationFor(*instrPtr);
  auto args = GetInstrArgsFor(*instrPtr);
  //create first node of cfgpart
  LlvmCfgNode& firstNode = LlvmCfgNode::CreateNode(op, args, *instrPtr);

  //prepare for following instructions
  currentNode = &firstNode;
  instrPtr = instrPtr->getNextNode();

  //all following instructions in BB except for the last one
  //add them to CFG
  while (!instrPtr->isTerminator())
  {
    auto& op = GetOperationFor(*instrPtr);
    auto args = GetInstrArgsFor(*instrPtr);
    currentNode = &currentNode->InsertNewAfter(op, args, *instrPtr);
    instrPtr = instrPtr->getNextNode();
  }
  //last BB instruction
  {
    // If the instruction is terminator, links go to the next block
    // @TODO Supports only ret and br (conditional and unconditional)

    //get matching op
    auto& op = GetOperationFor(*instrPtr);
    auto args = GetInstrArgsFor(*instrPtr);

    switch (instrPtr->getOpcode())
    {
    case llvm::Instruction::Br:
    {
      auto branchIsntrPtr = static_cast<const llvm::BranchInst*>(instrPtr);

      if (branchIsntrPtr->isUnconditional())
      {
        // Unconditional branch - next is first instruction of target basic block
        currentNode = &currentNode->InsertNewAfter(op, args, *instrPtr);
        LinkWithOrPlanProcessing(currentNode, branchIsntrPtr->getSuccessor(0), 0);
      }
      else //if (branchIsntrPtr->isConditional()) 
      {
        // Conditional branch - has two successors for true and false branches
        currentNode = &currentNode->InsertNewBranchAfter(op, args, *instrPtr);
        LinkWithOrPlanProcessing(currentNode, branchIsntrPtr->getSuccessor(0), 0);
        LinkWithOrPlanProcessing(currentNode, branchIsntrPtr->getSuccessor(1), 1);
      }
    }
    default:
      break;
    } // end of swtich

  }

  return firstNode;
}

ICfgNode& LlvmCfgParser::ParseModule(const llvm::Module& module)
{
  // Get first instruction of function main
  auto& entryBlock = module.getFunction("main")->getEntryBlock();

  auto& firstNode = ParseBasicBlock(&entryBlock);

  while (!parseAndLinkTogether.empty())
  {
    const llvm::BasicBlock* blockToParse;
    LlvmCfgNode* prevNodeToLink;
    unsigned targetIndexInNode;

    tie(blockToParse, prevNodeToLink, targetIndexInNode) = parseAndLinkTogether.front();
    parseAndLinkTogether.pop();

    auto& blockStartNode = ParseBasicBlock(blockToParse);
    LlvmCfgNode::LinkTogether(*prevNodeToLink, blockStartNode, targetIndexInNode);
  }
  return firstNode;
}

uptr<llvm::Module> LlvmCfgParser::OpenIrFile(string fileName)
{
  llvm::SMDiagnostic err;
  llvm::LLVMContext &context = *new llvm::LLVMContext();

  auto tmp = llvm::parseIRFile(fileName, err, context);
  //string msg{err.getMessage().str()};
  return tmp;
}



uptr<llvm::Module> moduleHandle;
ICfgNode& LlvmCfgParser::ParseAndOpenIrFile(string fileName)
{
  moduleHandle = OpenIrFile(fileName);
  return ParseModule(*moduleHandle);
}
