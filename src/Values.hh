#pragma once

#include "Definitions.hh"
#include "General.hh"

#include "enum_flags.h"

#include <boost/logic/tribool.hpp>

class ValueId {
  // Fields
private:
  uint64_t id;
  static ValueId nextIdToGive;

  // Constructors
public:
  /**/     ValueId()            : id{ 0 }  { }
  explicit ValueId(uint64_t id) : id{ id } { }

  // Conversion methods
  explicit operator uint64_t() const { return id; }

  // Creator methods
  static ValueId GetNextId() { return nextIdToGive++; }

  // Methods
  ValueId operator++()    { id++; return *this; } // prefix
  ValueId operator++(int) { auto copy = *this; id++; return copy; } // postfix

  bool operator==(const ValueId& other) const { return this->id == other.id; }
  bool operator!=(const ValueId& other) const { return this->id != other.id; }
  bool operator< (const ValueId& other) const { return this->id < other.id; }
  bool operator> (const ValueId& other) const { return this->id > other.id; }
  bool operator<=(const ValueId& other) const { return this->id <= other.id; }
  bool operator>=(const ValueId& other) const { return this->id >= other.id; }

};

/// <summary>
/// Class holding a set of values / their constraint based representations.
/// Internaly everything is a string of bits.
/// All integer values are reperesented in two's complement code
/// 
/// Arithemtic and shift operations which tends to have separated signed and unsigned instructions 
/// are abstracted into single operation. 
/// Also note, wrapping/overflow of certain operations is undefined behaviour in some languages.
/// Use the <paramref name="flags"/> parameter to specify wanted behaviour in these cases.
/// </summary>
class ValueContainer {
public:
  boost::tribool  Cmp       (ValueId first, ValueId second, Type type, uint32_t flags) const;
  boost::tribool  Cmp       (ValueId first, ValueId second, Type type) const; // signed by default
  boost::tribool  IsEq      (ValueId first, ValueId second, Type type) const;
  boost::tribool  IsNeq     (ValueId first, ValueId second, Type type) const;
  boost::tribool	IsBinaryEq(ValueId first, ValueId second) const;
  boost::tribool	IsUnknown (ValueId first) const;
  boost::tribool	IsZero    (ValueId first) const;

  ValueId Add(ValueId first, ValueId second, uint32_t flags, Type type);
  ValueId Sub(ValueId first, ValueId second, uint32_t flags, Type type);
  ValueId Mul(ValueId first, ValueId second, uint32_t flags, Type type);
  ValueId Div(ValueId first, ValueId second, uint32_t flags, Type type);
  ValueId Rem(ValueId first, ValueId second, uint32_t flags, Type type);

  ValueId LSh(ValueId first, ValueId second, uint32_t flags, Type type);
  ValueId RSh(ValueId first, ValueId second, uint32_t flags, Type type); // Arit/Logic, jinak jen exact

  ValueId	BinAnd(ValueId first, ValueId second, Type type);
  ValueId BinXor(ValueId first, ValueId second, Type type);
  ValueId BinOr(ValueId first, ValueId second, Type type);

  ValueId	ConvIntToFloat(ValueId first, uint32_t flags);
  ValueId	ConvFloatToInt(ValueId first, uint32_t flags);

  ValueId CreateIntVal();
  ValueId CreateFloatVal();

  ValueId CreateConstIntVal  (uint64_t value, Type type);
  ValueId CreateConstFloatVal(uint64_t value, Type type);
};

ENUM_FLAGS_EX(ValueOperationsFlags, int)
enum class ValueOperationsFlags {
  Signed         = 0x0001,
  Unsigned       = 0x0002,
  Exact          = 0x0004,
  NoSignedWrap   = ValueOperationsFlags::,
  NoUnsignedWrap = 0x0002
};

ENUM_FLAGS_EX(CmpFlags, int)
enum class CmpFlags {
  Default   = 0x0000,
  Signed    = CmpFlags::Default,
  Unsigned  = 0x0001,
  Eq        = 0x0002,
  Neq       = 0x0004,
  Gt        = 0x0008,
  GtEq      = CmpFlags::Gt | CmpFlags::Eq,
  Lt        = 0x0010,
  LtEq      = CmpFlags::Lt | CmpFlags::Eq,
  Float     = 0x0020,
  Ordered   = CmpFlags::Default,
  Unordered = 0x0040,
};


//class ValueRep {
//private:
//  ValueId id;
//  ValueContainer& vc;
//
//public:
//  /*ctr*/ ValueRep(void) {};
//
//  boost::tribool	IsBinaryEq(const ValueRep& other) const {}
//  boost::tribool	IsUnknown() const {}
//  boost::tribool	IsZero() const {}
//
//  /* void			SetConstraint(ConstraintType type, ValueRep &other) {}
//  void			SetConstraint(ConstraintType type, int64_t constValue) {}*/
//};

//class TypedValueRep : ValueRep {
//public:
//  Type type;
//
//  /*ctr*/ TypedValueRep(const Type& type) : type{type} {};
//
//  std::string		  ToString() const {}
//
//  boost::tribool  operator<(const TypedValueRep& other) const {}
//  boost::tribool	operator>(const TypedValueRep& other) const {}
//  boost::tribool	operator<=(const TypedValueRep& other) const {}
//  boost::tribool	operator>=(const TypedValueRep& other) const {}
//  boost::tribool	IsEq(const TypedValueRep& other) const {}
//  boost::tribool	IsNeq(const TypedValueRep& other) const {}
//
//  TypedValueRep	 operator+(const TypedValueRep& other) const {}
//  TypedValueRep	 operator-(const TypedValueRep& other) const {}
//  TypedValueRep	 operator*(const TypedValueRep& other) const {}
//  TypedValueRep	 operator/(const TypedValueRep& other) const {}
//  TypedValueRep	 operator%(const TypedValueRep& other) const {}
//};
//
//


// Enum class representing Constraint Types - < > <= >= == !=
enum class ConstraintType {
  LT = 0,
  GT,
  LTE,
  GTE,
  EQ,
  NEQ,
};
