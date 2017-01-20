#include "StateStorage.hh"

vector<uptr<IState>> StateManger::statePool{};
ref_deque<IState> StateManger::worklist{};