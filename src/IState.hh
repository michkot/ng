#pragma once

#include "Definitions.hh"
#include "General.hh"

class ICfgNode; //forward delcaration -- include colision ICfgNode vs IState
//#include "ICfgNode.hh"

using namespace ::std;

enum class StateCondition {
  New = 0,
  Explored = 1,
  AbstractedOut = 2
};

class IState {
public:
  // informace o posledni provedene op. na ktere je stav zalozen?

  StateCondition condition = StateCondition::New;

  ICfgNode& lastCfgNode; //optional
  ICfgNode& nextCfgNode; //mandatory

  ref_vector<IState> prevStates; //optional

  virtual void SetExplored() { condition = StateCondition::Explored; }
  virtual bool IsNew() { return condition == StateCondition::New; }

  virtual void AddGlobalVar(OperArg var, VariableDebugInfo info = VariableDebugInfo{}) = 0;
  virtual void AddLocalVar(OperArg var, VariableDebugInfo info = VariableDebugInfo{}) = 0;
  //pro dead value analysis / memory leaks
  virtual void DelLocalVar(OperArg var, VariableDebugInfo info = VariableDebugInfo{}) = 0;

  //předávané argumenty, návratový typ, návratová lokace/instrukce
  virtual void PushFrame(FunctionCallInfo info) = 0;
  //pozn lokace návratu musí být uložena ve stavu, na adekvátní urovni, přilepena na stack frame
  virtual void PopFrame(OperArg retVar) = 0;

protected:
  /*ctr*/ IState(ICfgNode& lastNode, ICfgNode& nextNode) :
    lastCfgNode{lastNode},
    nextCfgNode{nextNode}
  {
  }
};
