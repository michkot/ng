#pragma once

#include <map>
#include "Definitions.hh"
#include "General.hh"
#include "IOperation.hh"
#include "Values.hh"
#include "IState.hh"

using namespace ::std;

typedef int DummyValueId;

class DummyState : public IState {
private:
  map<ValueId, ValueRep> globalVarMapping;
  map<ValueId, ValueRep> varMapping;

public:
  virtual void AddGlobalVar(InstrArg var, VariableDebugInfo info = VariableDebugInfo{}) override
  {
    globalVarMapping.insert_or_assign(var.id, ValueRep{var.type});
  }

  virtual void AddLocalVar(InstrArg var, VariableDebugInfo info = VariableDebugInfo{}) override
  {
    varMapping.insert_or_assign(var.id, ValueRep{var.type});
  }

  virtual void DelLocalVar(InstrArg var, VariableDebugInfo info = VariableDebugInfo{}) override
  {
    throw runtime_error("not implemented");
  }
  virtual void PushFrame(FunctionCallInfo info) override { throw runtime_error("not implemented"); }
  virtual void PopFrame(InstrArg retVar) override { throw runtime_error("not implemented"); }

  /*ctr*/ DummyState(ICfgNode& lastNode, ICfgNode& nextNode) :
    globalVarMapping{},
    varMapping{},
    IState(lastNode, nextNode)
  { }

};