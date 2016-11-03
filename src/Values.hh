#pragma once

#include "Definitions.hh"
#include "General.hh"

#include <boost/logic/tribool.hpp>

class ValueRep {
public:
  /*ctr*/ ValueRep(void) {};

  std::string		  ToString() const {}

  boost::tribool  IsLt(const ValueRep& other, Type type) const {}
  boost::tribool	IsGt(const ValueRep& other, Type type) const {}
  boost::tribool	IsLtE(const ValueRep& other, Type type) const {}
  boost::tribool	IsGtE(const ValueRep& other, Type type) const {}
  boost::tribool	IsEq(const ValueRep& other, Type type) const {}
  boost::tribool	IsNeq(const ValueRep& other, Type type) const {}

  boost::tribool	IsBinaryEq(const ValueRep& other) const {}
  boost::tribool	IsUnknown() const {}
  boost::tribool	IsZero() const {}

  ValueRep    	  Add(const ValueRep& other, Type type) const {}
  ValueRep    	  Sub(const ValueRep& other, Type type) const {}
  ValueRep    	  Mul(const ValueRep& other, Type type) const {}
  ValueRep    	  Div(const ValueRep& other, Type type) const {}
  ValueRep    	  Rmn(const ValueRep& other, Type type) const {}

  ValueRep	      BinAnd(const ValueRep& other) const {}
  ValueRep	      BinOr(const ValueRep& other) const {}
  ValueRep    	  BinLSh(const ValueRep& other, Type type) const {}
  ValueRep    	  BinRSh(const ValueRep& other, Type type) const {}

  /* void			SetConstraint(ConstraintType type, ValueRep &other) {}
  void			SetConstraint(ConstraintType type, int64_t constValue) {}*/
};

class TypedValueRep : ValueRep {
public:
  Type type;

  /*ctr*/ TypedValueRep(const Type& type) : type{type} {};

  std::string		  ToString() const {}

  boost::tribool  operator<(const TypedValueRep& other) const {}
  boost::tribool	operator>(const TypedValueRep& other) const {}
  boost::tribool	operator<=(const TypedValueRep& other) const {}
  boost::tribool	operator>=(const TypedValueRep& other) const {}
  boost::tribool	IsEq(const TypedValueRep& other) const {}
  boost::tribool	IsNeq(const TypedValueRep& other) const {}

  TypedValueRep	 operator+(const TypedValueRep& other) const {}
  TypedValueRep	 operator-(const TypedValueRep& other) const {}
  TypedValueRep	 operator*(const TypedValueRep& other) const {}
  TypedValueRep	 operator/(const TypedValueRep& other) const {}
  TypedValueRep	 operator%(const TypedValueRep& other) const {}
};


/*
* Enum class representing Constraint Types - < > <= >= == !=
*/
enum class ConstraintType {
  LT = 0,
  GT,
  LTE,
  GTE,
  EQ,
  NEQ,
};
