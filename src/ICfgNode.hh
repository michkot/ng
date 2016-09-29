#pragma once

#include "Definitions.hh"
#include "General.hh"
#include "IOperation.hh"

class IState; //forward delcaration -- include colision ICfgNode vs IState
#include "IState.hh"
#include "StateStorage.hh"


class CfgNode;
class LlvmCfgNode;


//class INavigation {
//public:
//  virtual bool HasTwoNext();
//  virtual INavigation& GetNext();
//  virtual INavigation& GetNextTrue();
//  virtual INavigation& GetNextFalse();
//  virtual const ref_vector<INavigation> GetPrevs(); //?
//
//  virtual void GetDebugInfo();
//  virtual vector<InstrArg> GetArguments();
//};


class ICfgNode : public IOperation {
  friend CfgNode;
  friend LlvmCfgNode;
public:
  virtual bool HasTwoNext() { return false; }
  virtual ICfgNode& GetNext() const = 0;
  virtual ICfgNode& GetNextTrue() const = 0;
  virtual ICfgNode& GetNextFalse() const = 0;
  virtual const ref_vector<ICfgNode> GetPrevs() const = 0;

  virtual StateManger GetStates() = 0;
  virtual void GetDebugInfo() const = 0;
  virtual vector<InstrArg> GetArguments() const = 0;

  void Execute(IState& s, const vector<InstrArg>& args) override = 0;
  void Execute(IState& s)
  {
    return Execute(s, GetArguments());
  }




  ICfgNode& operator=(ICfgNode&) = delete;
  ICfgNode& operator=(ICfgNode&&) = delete;

protected:
  ICfgNode* next;
  ICfgNode* nextFalse = nullptr;
  ref_vector<ICfgNode> prevs;

  vector<InstrArg> args;

  virtual bool IsStartNode() { return false; }
  virtual bool IsTerminalNode() { return false; }

  /*ctr*/ ICfgNode() : next{nullptr}, prevs{} {}
  /*ctr*/ ICfgNode(ICfgNode* next) : next{next}, prevs{} {}
  /*ctr*/ ICfgNode(ref_vector<ICfgNode> prevs) : next{nullptr}, prevs{prevs} {}
  /*ctr*/ ICfgNode(ICfgNode* next, ref_vector<ICfgNode> prevs) : next{next}, prevs{prevs} {}
};


class StartCfgNode : public ICfgNode {
  friend CfgNode;
  friend LlvmCfgNode;
public:
  ICfgNode& GetNext() const override { return *next; }
  ICfgNode& GetNextTrue() const override { throw runtime_error{"NS"}; }
  ICfgNode& GetNextFalse() const override { throw runtime_error{"NS"}; }
  const ref_vector<ICfgNode> GetPrevs() const override { throw runtime_error{"NS"}; }

  StateManger GetStates() override { throw runtime_error{"NS"}; }
  void GetDebugInfo() const override { throw runtime_error{"NS"}; }
  vector<InstrArg> GetArguments() const override { throw runtime_error{"NS"}; }

  bool IsStartNode() override { return true; }

  void Execute(IState& s, const vector<InstrArg>& args) override
  {
    throw runtime_error{"NS"};
  }

private:
  /*ctr*/ StartCfgNode() {}
};

class TerminalCfgNode : public ICfgNode {
  friend CfgNode;
  friend LlvmCfgNode;
public:
  ICfgNode& GetNext() const override { throw runtime_error{"NS"}; }
  ICfgNode& GetNextTrue() const override { throw runtime_error{"NS"}; }
  ICfgNode& GetNextFalse() const override { throw runtime_error{"NS"}; }
  const ref_vector<ICfgNode> GetPrevs() const override { return prevs; }

  StateManger GetStates() override { throw runtime_error{"NS"}; }
  void GetDebugInfo() const override { throw runtime_error{"NS"}; }
  vector<InstrArg> GetArguments() const override { throw runtime_error{"NS"}; }

  bool IsTerminalNode() override { return true; }

  void Execute(IState& s, const vector<InstrArg>& args) override
  {
    throw runtime_error{"NS"};
  }

private:
  /*ctr*/ TerminalCfgNode() {}
};


#include <llvm/IR/Module.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>

//uses llvm::Instruction*
class CfgNode : public ICfgNode {
private:
  IOperation& op;
  StateManger states;

public:
  bool HasTwoNext() override { return nextFalse != nullptr; }
  ICfgNode& GetNext() const override { return *next; }
  ICfgNode& GetNextTrue() const override
  {
    if (nextFalse == nullptr)
      throw runtime_error("not branch");
    return *next;
  }
  ICfgNode& GetNextFalse() const override
  {
    if (nextFalse == nullptr)
      throw runtime_error("not branch");
    return *nextFalse;
  }
  const ref_vector<ICfgNode> GetPrevs() const override { return prevs; }

  void Execute(IState& s, const vector<InstrArg>& args) override
  {
    return op.Execute(s, args);
  }
  StateManger GetStates() override { return states; }
  vector<InstrArg> GetArguments() const override { return args; }

protected:
  /*ctr*/ CfgNode(IOperation& op, ICfgNode& prev, ICfgNode& next) :
    op{op},
    ICfgNode(&next, ref_vector<ICfgNode>{1, prev})
  {
  }

  // ---------------------- for dev needs only ---------------------------- //
public:
  virtual void GetDebugInfo() const override { throw runtime_error("not implemented"); }

  static CfgNode& CreateNode(IOperation& op)
  {
    CfgNode* newNode = new CfgNode{op, *new StartCfgNode{}, *new TerminalCfgNode{}};
    ((ICfgNode&)newNode->prevs[0]).next = newNode;
    newNode->next->prevs.push_back(*newNode);
    return *newNode;
  }

  //beware - adding a node after terminal node here(after inproper cast) would not raise exception
  CfgNode& InsertNewAfter(IOperation& op)
  {
    CfgNode* newNode = new CfgNode{op, *this, *this->next};
    this->next = newNode;
    return *newNode;
  }
  // ---------------------- for dev needs only ---------------------------- //
};


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

  virtual void GetDebugInfo() const override { throw runtime_error("not implemented"); }
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
      throw runtime_error("unsupported targetIndex value");
  }
};