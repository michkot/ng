#pragma once

#include "Definitions.hh"
#include "General.hh"

class ValueRep {
public:
  Type type;
  uint64_t value;

  /*ctr*/ ValueRep(const Type& type) : type{type}, value{} {};
};