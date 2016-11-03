#pragma once

#include <map>
#include "Definitions.hh"
#include "General.hh"
#include "IOperation.hh"
#include "Values.hh"
#include "IState.hh"

typedef int DummyValueId;

class DummyState : public IState {
private:
  std::map<ValueId, TypedValueRep> globalVarMapping;
  std::map<ValueId, TypedValueRep> varMapping;

public:
  virtual void AddGlobalVar(OperArg var, VariableDebugInfo info = VariableDebugInfo{}) override
  {
    globalVarMapping.insert_or_assign(var.id, TypedValueRep{var.type});
  }

  virtual void AddLocalVar(OperArg var, VariableDebugInfo info = VariableDebugInfo{}) override
  {
    varMapping.insert_or_assign(var.id, TypedValueRep{var.type});
  }

  virtual void DelLocalVar(OperArg var, VariableDebugInfo info = VariableDebugInfo{}) override
  {
    throw NotImplementedException{"DelLocalVar"};
  }
  virtual void PushFrame(FunctionCallInfo info) override
  {
    throw NotImplementedException{"PushFrame"};
  }
  virtual void PopFrame(OperArg retVar) override
  {
    throw NotImplementedException{"PopFrame"};
  }

  /*ctr*/ DummyState(ICfgNode& lastNode, ICfgNode& nextNode) :
    IState(lastNode, nextNode),
    globalVarMapping{},
    varMapping{}
  { }

};