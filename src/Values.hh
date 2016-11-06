#pragma once

#include "Definitions.hh"
#include "Type.hh"

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

ENUM_FLAGS(ArithFlags)
enum class ArithFlags {
  Default        = 0x0000,
  Signed         = 0x0001,
  Unsigned       = 0x0002,
  Exact          = 0x0004,
  NoSignedWrap   = 0x0008,
  NoUnsignedWrap = 0x0010,
  FmfFlag1       = 0x0010,
  FmfFlag2       = 0x0020,
  FmfFlag3       = 0x0040,
  FmfFlag4       = 0x0080,
  FmfFlag5       = 0x0100,
};

ENUM_FLAGS(CmpFlags)
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
  SigGt     = CmpFlags::Signed   | CmpFlags::Gt,
  SigGtEq   = CmpFlags::Signed   | CmpFlags::GtEq,
  SigLt     = CmpFlags::Signed   | CmpFlags::Lt,
  SigLtEq   = CmpFlags::Signed   | CmpFlags::LtEq,
  UnsigGt   = CmpFlags::Unsigned | CmpFlags::Gt,
  UnsigGtEq = CmpFlags::Unsigned | CmpFlags::GtEq,
  UnsigLt   = CmpFlags::Unsigned | CmpFlags::Lt,
  UnsigLtEq = CmpFlags::Unsigned | CmpFlags::LtEq,
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
  // parametrzied
  boost::tribool IsCmp     (ValueId first, ValueId second, Type type, CmpFlags flags) const;
  // signed "C style" by default
  boost::tribool IsCmp     (ValueId first, ValueId second, Type type) const;
  boost::tribool IsEq      (ValueId first, ValueId second, Type type) const;
  boost::tribool IsNeq     (ValueId first, ValueId second, Type type) const;
  boost::tribool IsTrue    (ValueId first, Type type) const;
  boost::tribool IsFalse   (ValueId first, Type type) const;

  boost::tribool IsBinaryEq(ValueId first, ValueId second) const;
  boost::tribool IsUnknown (ValueId first) const;
  boost::tribool IsZero    (ValueId first) const;

  ValueId SetCmp(ValueId first, ValueId second, Type type, CmpFlags flags);

  ValueId Add   (ValueId first, ValueId second, Type type, ArithFlags flags);
  ValueId Sub   (ValueId first, ValueId second, Type type, ArithFlags flags);
  ValueId Mul   (ValueId first, ValueId second, Type type, ArithFlags flags);
  ValueId Div   (ValueId first, ValueId second, Type type, ArithFlags flags);
  ValueId Rem   (ValueId first, ValueId second, Type type, ArithFlags flags);

  ValueId LSh   (ValueId first, ValueId second, Type type, ArithFlags flags);
  ValueId RSh   (ValueId first, ValueId second, Type type, ArithFlags flags);

  ValueId LogAnd(ValueId first, ValueId second, Type type, ArithFlags flags);
  ValueId LogOr (ValueId first, ValueId second, Type type, ArithFlags flags);
  ValueId LogXor(ValueId first, ValueId second, Type type, ArithFlags flags);
  ValueId LogNot(ValueId first, ValueId second, Type type, ArithFlags flags);

  ValueId BitAnd(ValueId first, ValueId second, Type type);
  ValueId BitOr (ValueId first, ValueId second, Type type);
  ValueId BitXor(ValueId first, ValueId second, Type type);
  ValueId BitNot(ValueId first, Type type);

  ValueId ConvIntToFloat(ValueId first, uint32_t flags);
  ValueId ConvFloatToInt(ValueId first, uint32_t flags);

  ValueId CreateVal();

  ValueId CreateConstIntVal  (uint64_t value, Type type);
  //TODO: Maybe remove, because value should be binary representation
  ValueId CreateConstIntVal  (int64_t  value, Type type);
  ValueId CreateConstFloatVal(float    value, Type type);
  ValueId CreateConstFloatVal(double   value, Type type);
};