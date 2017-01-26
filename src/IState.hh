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

//TODO: probably rename lastCfgNode, nextCfgNode, previousStates

class IState {
public:
  // informace o posledni provedene op. na ktere je stav zalozen?

  StateCondition condition = StateCondition::New;

  ICfgNode& lastCfgNode; // optional, this state is has been created in an execution of this program node
  ICfgNode& nextCfgNode; // mandatory, this program node should be executed on this to futrher advance

  // optional, all states that participated on creating this state (JOIN)
  // might be changed to Get/Set property
  ref_vector<IState> previousStates; 
  // optional, all states that are based on this state
  // might be changed to Get/Set property
  ref_vector<IState> followingStates; 

  virtual ~IState() {}

  virtual void SetExplored() { condition = StateCondition::Explored; }
  virtual bool IsNew() { return condition == StateCondition::New; }

  virtual void AddGlobalVar(OperArg var) = 0;
  virtual void AddLocalVar(OperArg var) = 0;
  // pro dead value analysis / memory leaks:
  // momentálně pro toto nevidím use-case, neboť nemám jak zjistit že je proměná (resp její SSA následník)
  // již out of scope.
  virtual void DelLocalVar(OperArg var) = 0; 

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
