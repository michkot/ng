#include "StateStorage.hh"

vector<uptr<IState>> StateManger::statePool = vector<uptr<IState>>{};
ref_list<IState> StateManger::worklist = ref_list<IState>{};