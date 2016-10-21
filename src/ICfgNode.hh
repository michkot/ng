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
//  virtual vector<OperArg> GetArguments();
//};


class ICfgNode : public IOperation {
  friend CfgNode;
  friend LlvmCfgNode;
public:
  virtual bool HasTwoNext() { return false; }
  virtual ICfgNode& GetNext() const = 0;
  virtual ICfgNode& GetNextTrue() const = 0;
  virtual ICfgNode& GetNextFalse() const = 0;
  virtual const ref_vector<ICfgNode>& GetPrevs() const = 0;

  virtual StateManger GetStates() = 0;
  virtual void GetDebugInfo() const = 0;
  virtual vector<OperArg> GetArguments() const = 0;

  void Execute(IState& s, const vector<OperArg>& args) override = 0;
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

  virtual bool IsStartNode() { return false; }
  virtual bool IsTerminalNode() { return false; }

  /*ctr*/ ICfgNode() : next{nullptr}, prevs{} {}
  /*ctr*/ ICfgNode(ICfgNode* next) : next{next}, prevs{} {}
  /*ctr*/ ICfgNode(ref_vector<ICfgNode> prevs) : next{nullptr}, prevs{prevs} {}
  /*ctr*/ ICfgNode(ICfgNode* next, ref_vector<ICfgNode> prevs) : next{next}, prevs{prevs} {}
  /*dst*/ virtual ~ICfgNode() = default;
};


class StartCfgNode : public ICfgNode {
  friend CfgNode;
  friend LlvmCfgNode;
public:
  ICfgNode& GetNext() const override { return *next; }
  ICfgNode& GetNextTrue() const override { throw NotSupportedException{}; }
  ICfgNode& GetNextFalse() const override { throw NotSupportedException{}; }
  const ref_vector<ICfgNode>& GetPrevs() const override { throw NotSupportedException{}; }

  StateManger GetStates() override { throw NotSupportedException{}; }
  void GetDebugInfo() const override { throw NotSupportedException{}; }
  vector<OperArg> GetArguments() const override { throw NotSupportedException{}; }

  bool IsStartNode() override { return true; }

  void Execute(IState& s, const vector<OperArg>& args) override
  {
    throw NotSupportedException{};
  }

private:
  /*ctr*/ StartCfgNode() {}
};

class TerminalCfgNode : public ICfgNode {
  friend CfgNode;
  friend LlvmCfgNode;
public:
  ICfgNode& GetNext() const override { throw NotSupportedException{}; }
  ICfgNode& GetNextTrue() const override { throw NotSupportedException{}; }
  ICfgNode& GetNextFalse() const override { throw NotSupportedException{}; }
  const ref_vector<ICfgNode>& GetPrevs() const override { return prevs; }

  StateManger GetStates() override { throw NotSupportedException{}; }
  void GetDebugInfo() const override { throw NotSupportedException{}; }
  vector<OperArg> GetArguments() const override { throw NotSupportedException{}; }

  bool IsTerminalNode() override { return true; }

  void Execute(IState& s, const vector<OperArg>& args) override
  {
    throw NotSupportedException{};
  }

private:
  /*ctr*/ TerminalCfgNode() {}
};


class CfgNode : public ICfgNode {
private:
  vector<OperArg> args;
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

  const ref_vector<ICfgNode>& GetPrevs() const override { return prevs; }

  void Execute(IState& s, const vector<OperArg>& args) override
  {
    return op.Execute(s, args);
  }
  StateManger GetStates() override { return states; }
  vector<OperArg> GetArguments() const override { return args; }

protected:
  /*ctr*/ CfgNode(IOperation& op, vector<OperArg> args, ICfgNode& prev, ICfgNode& next) :
    op{op},
    args{args},
    ICfgNode(&next, ref_vector<ICfgNode>{1, prev})
  {
  }
//
//  // ---------------------- for dev needs only ---------------------------- //
//public:
//  virtual void GetDebugInfo() const override { throw NotImplementedException{}; }
//
//  static CfgNode& CreateNode(IOperation& op)
//  {
//    CfgNode* newNode = new CfgNode{op, *new StartCfgNode{}, *new TerminalCfgNode{}};
//    ((ICfgNode&)newNode->prevs[0]).next = newNode;
//    newNode->next->prevs.push_back(*newNode);
//    return *newNode;
//  }
//
//  //beware - adding a node after terminal node here(after inproper cast) would not raise exception
//  CfgNode& InsertNewAfter(IOperation& op)
//  {
//    CfgNode* newNode = new CfgNode{op, *this, *this->next};
//    this->next = newNode;
//    return *newNode;
//  }
//  // ---------------------- for dev needs only ---------------------------- //
};

