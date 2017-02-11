#include "StateStorage.hh"

vector<uptr<IState>> StatesManager::statePool{};
ref_deque<IState> StatesManager::worklist{};