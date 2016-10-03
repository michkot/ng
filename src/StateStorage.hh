#pragma once

#include "Definitions.hh"
#include "General.hh"
#include "IState.hh"

using namespace ::std;

class NewStateCfgNodePair {
public:
  uptr<IState> c; //newly created program state
  ICfgNode& l; //next node/operation to apply

  /*ctr*/ NewStateCfgNodePair(uptr<IState> c, ICfgNode& l) : c{move(c)}, l{l} {}
};

enum class WorklistPriority {
  Standard = 0
};

class StateManger {
  static vector<uptr<IState>> statePool;
  static ref_list<IState> worklist;

  ref_vector<IState> states;
private:

public:
  void InsertAndEnqueue(
    uptr<IState> statePtr,
    WorklistPriority prioroty = WorklistPriority::Standard
  )
  {
    if (false) //is state equal to to other states here == equality check
    {
      //throw it out completely
      return;
    }
    if (false) //we are able to join something?
    {
      //for all intersted states in join
      //set them to AbstractedOut
      //and add the result of join operation
    }

    //else -> just add it
    {
      IState& state = *statePtr;
      statePool.push_back(move(statePtr));
      states.push_back(state);

      //here we should utilize priorities
      worklist.push_back(state);
    }
  }
};