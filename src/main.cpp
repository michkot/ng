#include <cstdio>
#include <memory>

#include <functional>
#include <vector>
#include <queue>
#include <stack>
#include <tuple>

#include <boost\range.hpp>

#include <llvm/IR/IntrinsicInst.h>
//KISS!!!

//Q:
//je možné, že po naplánování stavu k dalšímu kroku dojde k pøidání dalšího kontextu do stejné lokace
//a k abstrakci tìcho SMG?
//pøedpoklad - ne?

//logické ohodnocení parametrù stavu
//je dosažitelný? -> hodnotový systém pøi pøidání podmínek, øíká nám že nelze
//došlo k chybì pamìti (predator works!!!)

//do budoucna - zmìnit flow nových stavù tak,
//aby tvùrce domény mohl mít plnou kontrolu na zpùsobem pøidávání do worklistu

//indirect call of malloc - jak se o nej postarat aby se dostal do analyzátoru tam kam patøí

//#include "core\LlvmLoader.hh"
//#include "core\LlvmSourceParser.hh"
//#include "domains\smg\SpcOperationsFactory.hh"


using namespace ::std;
//using namespace ::boost;

#include "Definitions.hh"
#include "General.hh"
#include "Values.hh"
#include "IState.hh"
#include "IOperation.hh"
#include "ICfgNode.hh"

#include <map>


#include <llvm/IR/Module.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>



#include <llvm/Support/raw_ostream.h>
std::string dbgstr;
llvm::raw_string_ostream dbgstr_rso(dbgstr);


class LlvmCfgParser {
  IOperationFactory& opFactory;

  map<const llvm::BasicBlock*, LlvmCfgNode*> copyMapping;
  queue<tuple<const llvm::BasicBlock*, LlvmCfgNode*, unsigned int>> parseAndLinkTogether;

public:
  LlvmCfgParser(IOperationFactory& opFactory) : opFactory{opFactory} {}

private:
  IOperation& GetOperationFor(const llvm::Instruction& instruction)
  {
    // Create correct operation
    IOperation* op;
    switch (instruction.getOpcode()) {
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

  static ValueType GetValueType(const llvm::Type&)
  {
    return ValueType{};
  }

  static ValueId GetValueId(int id)
  {
    return ValueId{(uint64_t)id};
  }
  static ValueId GetValueId(uint64_t id)
  {
    return ValueId{id};
  }
  static ValueId GetValueId(const llvm::Instruction* instr)
  {
    return ValueId{reinterpret_cast<uintptr_t>(instr)};
  }


  static vector<InstrArg> GetInstrArgsFor(const llvm::Instruction& instr)
  {
    vector<InstrArg> args;

    instr.print(llvm::errs(), true);
    llvm::errs() << "\n";

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
      for (int i = 0; i < num; i++)
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

  bool TryGetMappedCfgNode(const llvm::BasicBlock* bb, LlvmCfgNode** outNode)
  {    
    auto assignedNodeIt = copyMapping.find(bb);
    if (assignedNodeIt != copyMapping.end())
    {
      *outNode = (*assignedNodeIt).second;
      return true;
    }
    return false;
  }

  void LinkWithOrPlanProcessing(
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

  LlvmCfgNode& ParseBasicBlock(const llvm::BasicBlock* entryBlock) {
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
    while (!instrPtr->isTerminator()) {
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
      case llvm::Instruction::Br: {
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
      }

    }

    return firstNode;
  }

public:
  ICfgNode& ParseModule(const llvm::Module& module) 
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

  uptr<llvm::Module> OpenIrFile(string fileName)
  {
    llvm::SMDiagnostic err;
    llvm::LLVMContext &context = llvm::getGlobalContext();

    auto tmp = llvm::parseIRFile(fileName, err, context);
    //string msg{err.getMessage().str()};
    return tmp;
  }

};

//fronta stavù ke zpracování
ref_queue<IState> toProcess{};

void VerificationLoop()
{
  //smyèka
  while (!toProcess.empty())
  {
    //naèti stav
    IState& state = toProcess.front();
    toProcess.pop();

    //zpracuj stav
    {
      //vypoèet nových stavù

      //jaká je provádìná instrukce?
      //u podmínìného skoku - následují za ní metainstrukce/operace cíle skoku
      //které ovlivnují chování podle zvolené vìtve

      if (!state.IsNew())
        continue;

      state.nextCfgNode.Execute(state);


      /*
      if (state.l.HasTwoNext()) {
      auto& nextTrueLoc = state.l.GetNextTrue();
      auto statesTrue = nextTrueLoc.Execute(state.c);
      newStates.insert(
      newStates.end(),
      statesTrue.begin(),
      statesTrue.end()
      );

      auto& nextFalseLoc = state.l.GetNextFalse();
      auto statesFalse = nextFalseLoc.Execute(state.c);
      newStates.insert(
      newStates.end(),
      statesFalse.begin(),
      statesFalse.end()
      );
      }
      else {
      auto& nextLoc = state.l.GetNext();
      newStates = nextLoc.Execute(state.c);
      }*/

      ////modle podle veroniky::
      ////u podmínìný skoku 
      ////skoè na obì následující instrukce (basic blocky)
      ////a použí stejný kontext pro vytvoøení nových stavù

      ////u nepodmínìný skok
      ////skoè a použí stejný kontext

      ////u normální instrukce
      ////vyhodnot Execute

    }

  }
}

#include "DummyOperations.hh"

void Verify()
{
  /*auto f = DummyOperationFactory{};
  LlvmCfgParser parser{f};
  auto module = parser.OpenIrFile("input2.bc");
  auto& firstNode = parser.ParseModule(*module);

  auto emptyStateUPtr = make_unique<DummyState>(firstNode.GetPrevs()[0], firstNode);

  toProcess.push(*emptyStateUPtr);
*/
  VerificationLoop();
}

int main()
{
  Verify();
  return 0;

  printf("ahoj!");
  //getchar();

  /*auto dumm_op_ptr = make_unique<DummyOperation>();
  auto& dumm_op = *dumm_op_ptr;

  CfgNode& node1 = CfgNode::CreateNode(dumm_op);
  CfgNode& node2 = node1.InsertNewAfter(dumm_op);
  CfgNode& node3 = node2.InsertNewAfter(dumm_op);
  CfgNode& node4 = node3.InsertNewAfter(dumm_op);


  NewStateCfgNodePair p{make_unique<DummyState>(node1.GetPrevs()[0], node1), node1};
  
  NewStateCfgNodePair p2 = move(p);*/



  ////auto loader = LlvmLoader();
  ////auto module = loader.loadSource("input.bc");

  ////auto factory = unique_ptr<IOperationFactory>(new SpcOperationsFactory{ });  //make_unique<SpcOperationsFactory>();

  ////LlvmSourceParser parser(std::move(module), std::move(factory));
  ////auto firstNode = parser.parseCFG();

  return 0;
}