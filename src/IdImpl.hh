#pragma once
#include <cstdint>
#include <iostream>

template<typename T>
class Id {
private:
  // Fields
  static Id nextIdToGive;

public:
  uint64_t id;
  // Constructors
  constexpr Id()            : id{ 0 }  { }
  explicit  Id(uint64_t id) : id{ id } { }

  // Conversion methods
  explicit operator uint64_t() const { return id; }

  // Creator methods
  static Id GetNextId() { return nextIdToGive++; }

  // Methods
  Id operator++()    { id++; return *this; } // prefix
  Id operator++(int) { auto copy = *this; id++; return copy; } // postfix

  // Implementation of all comparsion operators (only the operator< is needed for most STD functionality) 
  //REVIEW: to use or not use the canonical way to implement the rest of operators
  //   canoncial way is to use the other already defined operators

  constexpr bool operator==(const Id& other) const { return this->id == other.id; } // canonicaly implemented
  constexpr bool operator!=(const Id& other) const { return this->id != other.id; }
  constexpr bool operator< (const Id& other) const { return this->id < other.id; }  // canonicaly implemented
  constexpr bool operator> (const Id& other) const { return this->id > other.id; }
  constexpr bool operator<=(const Id& other) const { return this->id <= other.id; }
  constexpr bool operator>=(const Id& other) const { return this->id >= other.id; }

  // Friend printer
  friend std::ostream& operator<<(std::ostream& os, const Id<T>& idValue) { return os << idValue.id; }
};
