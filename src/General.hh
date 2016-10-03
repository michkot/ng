#pragma once

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

class ValueType {
public:
  int blabla;
  int bleble;
  /*ctr*/ ValueType() : blabla{0}, bleble{0} {}
};

class InstrArg {
public:
  ValueId id;
  ValueType type;

  /*ctr*/ InstrArg(ValueId id, ValueType type) : id{id}, type{type} {}

};

//pøedávané argumenty, návratový typ, návratová lokace/instrukce
typedef int FunctionCallInfo;
typedef int FunctionInfo;

typedef int VariableDebugInfo;