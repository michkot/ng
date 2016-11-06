#pragma once

#include "Definitions.hh"
#include "General.hh"
#include "IOperation.hh"
#include "ICfgNode.hh"
#include "IState.hh"
#include "StateStorage.hh"
#include "DummyState.hh"

using namespace ::std;

class BaseOperation : public IOperation {
  virtual uptr<IState> ExecuteSimple(IState& s, const vector<OperArg>& args) = 0;
  //{
  //  return make_unique<DummyState>(s.nextCfgNode, s.nextCfgNode.GetNext());
  //}

  void Execute(IState& s, const vector<OperArg>& args) override
  {
    s.nextCfgNode.GetStates().InsertAndEnqueue(
      ExecuteSimple(s, args)
    );

    s.SetExplored();
    return;
  }
};

class DummyOperation : public IOperation {
public:
  void Execute(IState& s, const vector<OperArg>& args) override
  {
    if (s.nextCfgNode.HasTwoNext())
    {
      s.nextCfgNode.GetStates().InsertAndEnqueue(
        make_unique<DummyState>(s.nextCfgNode, s.nextCfgNode.GetNextTrue())
      );
      s.nextCfgNode.GetStates().InsertAndEnqueue(
        make_unique<DummyState>(s.nextCfgNode, s.nextCfgNode.GetNextFalse())
      );
    }
    else
    {
      s.nextCfgNode.GetStates().InsertAndEnqueue(
        make_unique<DummyState>(s.nextCfgNode, s.nextCfgNode.GetNext())
      );
    }

    s.SetExplored();
    return;
  }
};

class DummyOperationFactory : public IOperationFactory {
public:

  // Inherited via IOperationFactory
  virtual IOperation & Ret() override
  {
    return *new DummyOperation{};
  }

  virtual IOperation & Br() override
  {
    return *new DummyOperation{};
  }

  virtual IOperation & Add() override
  {
    return *new DummyOperation{};
  }

  virtual IOperation & Sub() override
  {
    return *new DummyOperation{};
  }

  virtual IOperation & Mul() override
  {
    return *new DummyOperation{};
  }

  virtual IOperation & Div() override
  {
    return *new DummyOperation{};
  }
  virtual IOperation & Rem() override
  {
    return *new DummyOperation{};
  }

  virtual IOperation & Shl() override
  {
    return *new DummyOperation{};
  }

  virtual IOperation & Shr() override
  {
    return *new DummyOperation{};
  }
  virtual IOperation & And() override
  {
    return *new DummyOperation{};
  }

  virtual IOperation & Or() override
  {
    return *new DummyOperation{};
  }

  virtual IOperation & Xor() override
  {
    return *new DummyOperation{};
  }

  virtual IOperation & Alloca() override
  {
    return *new DummyOperation{};
  }

  virtual IOperation & Load() override
  {
    return *new DummyOperation{};
  }

  virtual IOperation & Store() override
  {
    return *new DummyOperation{};
  }

  virtual IOperation & GetElementPtr() override
  {
    return *new DummyOperation{};
  }

  virtual IOperation & Cmp() override
  {
    return *new DummyOperation{};
  }

  virtual IOperation & NotSupportedInstr() override
  {
    return *new DummyOperation{};
  }

};
