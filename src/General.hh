#pragma once

#include "Type.hh"

#include <cstdint>

class ValueId {
  uint64_t inner;
public:
  bool operator<(const ValueId& right) const
  {
    return inner < right.inner;
  }

  /*ctr*/ explicit ValueId(uint64_t id) : inner{id} {}
};

class OperArg {
public:
  ValueId id;
  Type type;

  /*ctr*/ OperArg(ValueId id, Type type) : id{id}, type{type} {}

};

//předávané argumenty, návratový typ, návratová lokace/instrukce
typedef int FunctionCallInfo;
typedef int FunctionInfo;

typedef int VariableDebugInfo;