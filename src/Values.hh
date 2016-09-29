#pragma once

#include "Definitions.hh"
#include "General.hh"

using namespace ::std;

class ValueRep {
public:
  ValueType type;
  uint64_t value;

  /*ctr*/ ValueRep(const ValueType& type) : type{type}, value{} {};
};