#pragma once

#include "Definitions.hh"
#include "Type.hh"

#include "enum_flags.h"

#include <boost/logic/tribool.hpp>

//TODO: doplnit hlavicku -> jedna se o zapouzdreni integru pro typovou kontrolu
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

  //TODO: zredukovat pocet operatoru podle STD
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
  FmfFlag1       = 0x0020,
  FmfFlag2       = 0x0040,
  FmfFlag3       = 0x0080,
  FmfFlag4       = 0x0100,
  FmfFlag5       = 0x0200,
};

ENUM_FLAGS(CmpFlags)
enum class CmpFlags {
  Default   = 0x0000,
  //Signed    = CmpFlags::Default, // we can not mask 0x00!
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
  SigGt     = CmpFlags::Default  | CmpFlags::Gt,
  SigGtEq   = CmpFlags::Default  | CmpFlags::GtEq,
  SigLt     = CmpFlags::Default  | CmpFlags::Lt,
  SigLtEq   = CmpFlags::Default  | CmpFlags::LtEq,
  UnsigGt   = CmpFlags::Unsigned | CmpFlags::Gt,
  UnsigGtEq = CmpFlags::Unsigned | CmpFlags::GtEq,
  UnsigLt   = CmpFlags::Unsigned | CmpFlags::Lt,
  UnsigLtEq = CmpFlags::Unsigned | CmpFlags::LtEq,
};

//TODO: některé operace implementovat

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
class IValueContainer {
public:
  virtual boost::tribool IsCmp     (ValueId first, ValueId second, Type type, CmpFlags flags) const = 0;
  virtual boost::tribool IsEq      (ValueId first, ValueId second, Type type) const = 0;
  virtual boost::tribool IsNeq     (ValueId first, ValueId second, Type type) const = 0;
  virtual boost::tribool IsTrue    (ValueId first, Type type) const = 0; // !=0
  virtual boost::tribool IsFalse   (ValueId first, Type type) const = 0; // ==0

  //TODO: komentář - vhodně okomentovat; možná přejmenovat BitwiseEq nebo InternalRepEq
  virtual boost::tribool IsBinaryEq(ValueId first, ValueId second) const = 0;
  virtual boost::tribool IsUnknown (ValueId first) const = 0;
  virtual boost::tribool IsZero    (ValueId first) const = 0;

  // Creates new boolean (1bit integer) value expressing the constraint
  virtual ValueId Cmp        (ValueId first, ValueId second, Type type, CmpFlags flags) = 0;
  // Sets constraint on both values
  virtual void    Assume     (ValueId first, ValueId second, Type type, CmpFlags flags) = 0;
  virtual void    AssumeTrue (ValueId first) = 0; // Sets contraint: first != 0 ( == true )
  virtual void    AssumeFalse(ValueId first) = 0; // Sets contraint: first == 0 ( == false)

  virtual ValueId Add   (ValueId first, ValueId second, Type type, ArithFlags flags) = 0;
  virtual ValueId Sub   (ValueId first, ValueId second, Type type, ArithFlags flags) = 0;
  virtual ValueId Mul   (ValueId first, ValueId second, Type type, ArithFlags flags) = 0;
  virtual ValueId Div   (ValueId first, ValueId second, Type type, ArithFlags flags) = 0;
  virtual ValueId Rem   (ValueId first, ValueId second, Type type, ArithFlags flags) = 0;

  virtual ValueId LSh   (ValueId first, ValueId second, Type type, ArithFlags flags) = 0;
  virtual ValueId RSh   (ValueId first, ValueId second, Type type, ArithFlags flags) = 0;

  virtual ValueId BitAnd(ValueId first, ValueId second, Type type) = 0;
  virtual ValueId BitOr (ValueId first, ValueId second, Type type) = 0;
  virtual ValueId BitXor(ValueId first, ValueId second, Type type) = 0;
  virtual ValueId BitNot(ValueId first, Type type) = 0;

  //following are not an LLVM operations
  //when C/C++ is compiled in LLVM, everything is series of != 0 tests and branch/phi instructions
  virtual ValueId LogAnd(ValueId first, ValueId second, Type type) = 0;
  virtual ValueId LogOr (ValueId first, ValueId second, Type type) = 0;
  virtual ValueId LogNot(ValueId first, Type type) = 0;

  //TODO: možná Truncate?
  virtual ValueId ExtendInt(ValueId first, Type sourceType, Type targetType, ArithFlags flags) = 0;
  virtual ValueId TruncInt (ValueId first, Type sourceType, Type targetType) = 0;

  //virtual ValueId ConvIntToFloat(ValueId first, uint32_t flags) = 0;
  //virtual ValueId ConvFloatToInt(ValueId first, uint32_t flags) = 0;

  virtual ValueId CreateVal(Type type) = 0;

  //TODO: opět - implementovat jako NotImplemented
  virtual ValueId CreateConstIntVal  (uint64_t value, Type type) = 0;
  virtual ValueId CreateConstIntVal  (uint64_t value           ) = 0; // To be potentially removed
  virtual ValueId CreateConstFloatVal(float    value, Type type) = 0;
  virtual ValueId CreateConstFloatVal(double   value, Type type) = 0;

  virtual void PrintDebug() { throw NotImplementedException(); }
};
