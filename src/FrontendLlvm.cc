#include "FrontendLlvm.hh"

#include "ICfgNode.hh"
#include "ValuesZ3.hh"

#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>


#include <llvm/IR/Constants.h>

//std::string dbgstr;
//llvm::raw_string_ostream dbgstr_rso(dbgstr);


class LlvmCfgNode : public CfgNode {
private:
  const llvm::Instruction& innerInstruction;
  //const vector<OperArg>

  /*ctr*/ LlvmCfgNode(IOperation& op, vector<OperArg> args,
    const llvm::Instruction& inner,
    ICfgNode& prev,
    ICfgNode& next
  ) :
    innerInstruction{inner},
    CfgNode(op, args, prev, next) {
  }

public:

  virtual void GetDebugInfo() const override { throw NotImplementedException{}; }
  virtual vector<OperArg> GetArguments() const override { return vector<OperArg>(); }

  static LlvmCfgNode& CreateNode(IOperation& op, vector<OperArg> args, const llvm::Instruction& inner)
  {
    LlvmCfgNode* newNode = new LlvmCfgNode{op, args, inner, *new StartCfgNode{}, *new TerminalCfgNode{}};
    ((ICfgNode&)newNode->prevs[0]).next = newNode;
    newNode->next->prevs.push_back(*newNode);
    return *newNode;
  }

  //beware - adding a node after terminal node here(after inproper cast) would not raise exception
  //same applies for similar linking manipulation

  LlvmCfgNode& InsertNewAfter(IOperation& op, vector<OperArg> args, const llvm::Instruction& inner)
  {
    LlvmCfgNode* newNode = new LlvmCfgNode{op, args, inner, *this, *this->next};
    this->next = newNode;
    return *newNode;
  }

  LlvmCfgNode& InsertNewBranchAfter(IOperation& op, vector<OperArg> args, const llvm::Instruction& inner)
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


Type LlvmCfgParser::GetValueType(llvm::Type* type)
{
  return Type{type};
}

FrontendValueId LlvmCfgParser::GetValueId(uint64_t id)
{
  return FrontendValueId{id};
}
FrontendValueId LlvmCfgParser::GetValueId(const llvm::Value* instr)
{
  return FrontendValueId{reinterpret_cast<uintptr_t>(instr)};
}
FrontendValueId LlvmCfgParser::GetValueId(const llvm::Value& instr)
{
  return FrontendValueId{reinterpret_cast<uintptr_t>(&instr)};
}

OperArg LlvmCfgParser::ToOperArg(const llvm::Value* value)
{
  return OperArg{GetValueId(value), GetValueType(value->getType())};
}
OperArg LlvmCfgParser::ToOperArg(const llvm::Value& value)
{
  return ToOperArg(&value);
}
OperArg LlvmCfgParser::GetEmptyOperArg()
{
  return
    OperArg{GetValueId(
      static_cast<uint64_t>(0)),
    Type::CreateVoidType()
  };
}

OperArg LlvmCfgParser::GetFlagsOperArg(CmpFlags flags)
{
  return
    OperArg{GetValueId(
      static_cast<uint64_t>(flags)),
    Type::CreateVoidType()
  };
}

OperArg LlvmCfgParser::GetFlagsOperArg(ArithFlags flags)
{
  return
    OperArg{GetValueId(
      static_cast<uint64_t>(flags)),
    Type::CreateVoidType()
  };
}

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
  case llvm::Instruction::SDiv:
    op = &opFactory.Div();
    break;
  case llvm::Instruction::URem:
  case llvm::Instruction::SRem:
    op = &opFactory.Rem();
    break;

    // Bitwise binary instructions
  case llvm::Instruction::Shl:
    op = &opFactory.Shl();
    break;
  case llvm::Instruction::LShr:
  case llvm::Instruction::AShr:
    op = &opFactory.Shr();
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
  case llvm::Instruction::FCmp:
    op = &opFactory.Cmp();
    break;

  default:
    op = &opFactory.NotSupportedInstr();
  }

  return *op;
}
//non null!!
std::set<const llvm::Constant*> constantValuesToBeCreated;

vector<OperArg> LlvmCfgParser::GetOperArgsForInstr(const llvm::Instruction& instr)
{
  vector<OperArg> args;

  //we know that this isntructions outcome/value is never used
  //if (instr.user_empty())
  //{
  //  return args;
  //}

  instr.print(llvm::errs()/*, true*/);
  llvm::errs() << "\n";

  //add result value, if it is not void
  if (instr.getType()->isVoidTy())
  {
    args.push_back(GetEmptyOperArg());
  }
  else
  {
    args.push_back(ToOperArg(instr));
  }

  unsigned num = instr.getNumOperands();
  switch (instr.getOpcode())
  {
  case llvm::Instruction::Call:
  {
    auto& typedInstr = static_cast<const llvm::CallInst&>(instr);
    //First, parse the call target
    auto func = typedInstr.getCalledFunction();
    if (func != nullptr)
    { // call target is a function value
      args.push_back(ToOperArg(func));
    }
    else
    { // call target is a pointer to a function of some sort
      auto val = typedInstr.getCalledValue();
      args.push_back(ToOperArg(val));
    }

    //Then, parse operands
    unsigned imax = typedInstr.getNumArgOperands();
    for (unsigned i = 0; i < imax; ++i)
    {
      const auto argument = typedInstr.getArgOperand(i);
      //TODO: constants!!! like in default part of select
      args.push_back(ToOperArg(argument));
    }

    break;
  } //  case llvm::Instruction::Call:
  case llvm::Instruction::ICmp:
  {
    auto& typedInstr = static_cast<const llvm::ICmpInst&>(instr);
    CmpFlags flags = CmpFlags::Default;
    flags |= typedInstr.isSigned()        ? CmpFlags::Signed   : CmpFlags::Default;
    flags |= typedInstr.isUnsigned()      ? CmpFlags::Unsigned : CmpFlags::Default;
    flags |= typedInstr.isTrueWhenEqual() ? CmpFlags::Eq       : CmpFlags::Default;

    switch (typedInstr.getUnsignedPredicate())
    {
    case llvm::CmpInst::ICMP_NE:
      flags |= CmpFlags::Neq;
      break;
    case llvm::CmpInst::ICMP_UGT:
    case llvm::CmpInst::ICMP_UGE:
      flags |= CmpFlags::Gt;
      break;
    case llvm::CmpInst::ICMP_ULT:
    case llvm::CmpInst::ICMP_ULE:
      flags |= CmpFlags::Lt;
      break;
    default:
      break;
    }

    args.push_back(GetFlagsOperArg(flags));
    break;
  } // case llvm::Instruction::ICmp:
  case llvm::Instruction::FCmp:
  {
    auto& typedInstr = static_cast<const llvm::FCmpInst&>(instr);
    CmpFlags flags = CmpFlags::Default;
    flags |= CmpFlags::Float;

    throw NotImplementedException();

    args.push_back(GetFlagsOperArg(flags));
    break;
  } // case llvm::Instruction::ICmp:
  default:
  {
    if (llvm::isa<llvm::BinaryOperator>(instr))
    {
      auto& typedInstr = static_cast<const llvm::BinaryOperator&>(instr);
      ArithFlags flags = ArithFlags::Default;
      flags |= typedInstr.hasNoSignedWrap()   ? ArithFlags::NoSignedWrap   : ArithFlags::Default;
      flags |= typedInstr.hasNoUnsignedWrap() ? ArithFlags::NoUnsignedWrap : ArithFlags::Default;
      //TODO: Add more?

      switch (instr.getOpcode())
      {
      case llvm::Instruction::SDiv:
      case llvm::Instruction::AShr:
        flags |= typedInstr.isExact() ? ArithFlags::Exact : ArithFlags::Default;
      case llvm::Instruction::SRem:
        flags |= ArithFlags::Signed;
        break;
      case llvm::Instruction::UDiv:
      case llvm::Instruction::LShr:
        flags |= typedInstr.isExact() ? ArithFlags::Exact : ArithFlags::Default;
      case llvm::Instruction::URem:
        flags |= ArithFlags::Unsigned;
        break;
      default:
        break;
      }
      args.push_back(GetFlagsOperArg(flags));
    }
    else // add empty operand placeholder for flags/function target
    {
      args.push_back(GetEmptyOperArg());
    }

    for (unsigned i = 0; i < num; ++i)
    {
      const auto& operand = *instr.getOperand(i);
      if (llvm::isa<llvm::Constant>(operand))
      {
        constantValuesToBeCreated.insert(&static_cast<const llvm::Constant&>(operand));
      }
      args.emplace_back(ToOperArg(operand));
    }

    break;
  } // default:
  } // switch

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
  auto args = GetOperArgsForInstr(*instrPtr);
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
    auto args = GetOperArgsForInstr(*instrPtr);
    currentNode = &currentNode->InsertNewAfter(op, args, *instrPtr);
    instrPtr = instrPtr->getNextNode();
  }
  //last BB instruction
  {
    // If the instruction is terminator, links go to the next block
    // @TODO Supports only ret and br (conditional and unconditional)

    //get matching op
    auto& op = GetOperationFor(*instrPtr);
    auto args = GetOperArgsForInstr(*instrPtr);

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
    } // end of switch

  }

  return firstNode;
}

void LlvmCfgParser::DealWithConstants()
{
  IValueContainer& vc = *new Z3ValueContainer();

  for (auto x : constantValuesToBeCreated)
  {
    /**/ if (auto constInt = llvm::dyn_cast<llvm::ConstantInt>(x))
    {
      vc.CreateConstIntVal(constInt->getValue().getZExtValue(), Type{constInt->getType()});
    }
    else if (auto constFp = llvm::dyn_cast<llvm::ConstantFP>(x))
    {
      if (&constFp->getValueAPF().getSemantics() == &llvm::APFloat::IEEEdouble)
      {
        vc.CreateConstFloatVal(constFp->getValueAPF().convertToDouble(), Type{constFp->getType()});
      }
      else if (&constFp->getValueAPF().getSemantics() == &llvm::APFloat::IEEEsingle)
      {
        vc.CreateConstFloatVal(constFp->getValueAPF().convertToFloat(), Type{constFp->getType()});
      }
      else
        throw NotImplementedException();
    }
    else
      throw NotImplementedException();
  }
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

  DealWithConstants();

  return firstNode;
}

uptr<llvm::Module> LlvmCfgParser::OpenIrFile(string fileName)
{
  llvm::SMDiagnostic err;
  llvm::LLVMContext &context = *new llvm::LLVMContext();

  auto tmp = llvm::parseIRFile(fileName, err, context);
  if (tmp == nullptr)
  {
    string msg{err.getMessage().str()};
    throw msg;
  }
  return tmp;
}

uptr<llvm::Module> moduleHandle;
ICfgNode& LlvmCfgParser::ParseAndOpenIrFile(string fileName)
{
  moduleHandle = OpenIrFile(fileName);
  return ParseModule(*moduleHandle);
}
