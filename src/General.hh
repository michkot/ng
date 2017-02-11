#pragma once

#include <cstdint>

#include "Type.hh"

class FrontendValueId {
  // Fields
private:
  uint64_t id;

  // Constructors
private:
  /**/     FrontendValueId()            : id{ 0 }  { }
public:
  explicit FrontendValueId(uint64_t id) : id{ id } { }

  // Conversion methods
  explicit operator uint64_t() const { return id; }

  // Methods
  FrontendValueId operator++()    { id++; return *this; } // prefix
  FrontendValueId operator++(int) { auto copy = *this; id++; return copy; } // postfix

  bool operator==(const FrontendValueId& other) const { return this->id == other.id; }
  bool operator!=(const FrontendValueId& other) const { return this->id != other.id; }
  bool operator< (const FrontendValueId& other) const { return this->id < other.id; }
  bool operator> (const FrontendValueId& other) const { return this->id > other.id; }
  bool operator<=(const FrontendValueId& other) const { return this->id <= other.id; }
  bool operator>=(const FrontendValueId& other) const { return this->id >= other.id; }

};

class OperArg {
public:
  FrontendValueId id;
  Type type;

  /*ctr*/ OperArg(FrontendValueId id, Type type) : id{id}, type{type} {}

};

//předávané argumenty, návratový typ, návratová lokace/instrukce
typedef int FunctionCallInfo;
