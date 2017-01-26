#include "StateStorage.hh"

vector<uptr<IState>> StatesManger::statePool{};
ref_deque<IState> StatesManger::worklist{};