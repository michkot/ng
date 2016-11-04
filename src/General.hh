#pragma once

#include "Type.hh"

#include <cstdint>

class FrontendValueId {
  uint64_t inner;
public:
  bool operator<(const FrontendValueId& right) const
  {
    return inner < right.inner;
  }

  /*ctr*/ explicit FrontendValueId(uint64_t id) : inner{id} {}
};

class OperArg {
public:
  FrontendValueId id;
  Type type;

  /*ctr*/ OperArg(FrontendValueId id, Type type) : id{id}, type{type} {}

};

//předávané argumenty, návratový typ, návratová lokace/instrukce
typedef int FunctionCallInfo;
typedef int FunctionInfo;

typedef int VariableDebugInfo;