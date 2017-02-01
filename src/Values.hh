#pragma once

#include "Definitions.hh"
#include "Type.hh"

#include "enum_flags.h"

#include <boost/logic/tribool.hpp>

/// <summary>
/// ValueId class is an type-safe encapsulation of integer-based IDs for values
/// </summary>
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

  // Implementation of all comparsion operators (only the operator< is needed for most STD functionality) 
  //REVIEW: to use or not use the canonical way to implement the rest of operators
  bool operator==(const ValueId& other) const { return this->id == other.id; } // canonicaly implemented
  bool operator!=(const ValueId& other) const { return this->id != other.id; }
  bool operator< (const ValueId& other) const { return this->id < other.id; }  // canonicaly implemented
  bool operator> (const ValueId& other) const { return this->id > other.id; }
  bool operator<=(const ValueId& other) const { return this->id <= other.id; }
  bool operator>=(const ValueId& other) const { return this->id >= other.id; }

};

ENUM_FLAGS(ArithFlags)
enum class ArithFlags {
  Default        = 0x0000,
  Signed         = 0x0001, // Needed for divison and remainder
  Unsigned       = 0x0002, // Needed for divison and remainder
  Exact          = 0x0004,
  NoSignedWrap   = 0x0008, // Undefined behaviour on signed overflow
  NoUnsignedWrap = 0x0010, // Undefined behaviour on unsigned overflow
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

//TODO@michkot: Document a way one should implement the following interface/abstract class

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

  // --------------------------------------------------------------------------
  // Section A - constant methods, comparison queries
  
  // The first method is an universal method for comparison of two values
  // The others are just helper / convenient methods

  virtual boost::tribool IsCmp     (ValueId first, ValueId second, Type type, CmpFlags flags) const = 0;
  virtual boost::tribool IsEq      (ValueId first, ValueId second, Type type) const 
                                       { return IsCmp(first, second, type, CmpFlags::Eq); }
  virtual boost::tribool IsNeq     (ValueId first, ValueId second, Type type) const 
                                       { return IsCmp(first, second, type, CmpFlags::Neq); }
  virtual boost::tribool IsTrue    (ValueId first, Type type) const 
                                       { return IsCmp(first, GetZero(type), type, CmpFlags::Neq); } // !=0
  virtual boost::tribool IsFalse   (ValueId first, Type type) const 
                                       { return IsCmp(first, GetZero(type), type, CmpFlags::Eq); }  // ==0
  
  // Compares values' internal representations - typically a bitwise comparison of fixed-length integers
  virtual boost::tribool IsInternalRepEq(ValueId first, ValueId second) const = 0;
  // The value is zero in all possible interpretations 
  virtual boost::tribool IsZero    (ValueId first) const = 0;
  // There are no informations regarding this value
  virtual bool           IsUnknown(ValueId first) const = 0;

  // --------------------------------------------------------------------------
  // Section B - modifying methods, assumptions addition 

  // Creates new boolean (1bit integer) value expressing the constraint
  virtual ValueId Cmp        (ValueId first, ValueId second, Type type, CmpFlags flags) = 0;
  // Sets constraint on both values
  virtual void    Assume     (ValueId first, ValueId second, Type type, CmpFlags flags) = 0;
  // Sets contraint: the value is different from zero in all possible interpetations
  // Sets contraint: first != 0, i.e. == true
  virtual void    AssumeTrue (ValueId first) { throw NotImplementedException(); }
  // Sets contraint: the value is equal     to   zero in all possible interpetations
  // Sets contraint: first == 0, i.e. == false
  virtual void    AssumeFalse(ValueId first) { throw NotImplementedException(); }

  // --------------------------------------------------------------------------
  // Section C - modifying methods, math/logic/bitwise binary and unary operations

  // In following methods, the representation of result of specific operation is assigned to returned value 
  // (need not be a newly created one, e.g. typically in case of constants)

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

  // --------------------------------------------------------------------------
  // Section D - modifying methods, others

  // Following methods are not an LLVM operations
  // When C/C++ is compiled in LLVM, 
  // everything is converted into series of != 0 tests and branch/phi instructions
  // as an effect of expression shorthand evaluation

  virtual ValueId LogAnd(ValueId first, ValueId second, Type type) { throw NotImplementedException(); }
  virtual ValueId LogOr (ValueId first, ValueId second, Type type) { throw NotImplementedException(); }
  virtual ValueId LogNot(ValueId first, Type type)                 { throw NotImplementedException(); }

  // Following methods provides bitwise extension and truncation
  // The returned value need not be a newly created one,
  // i.e. with fixed 64-bit storage implementation, unsigned extensions and truncation would be no-op
  // Sign./unsig. extensions is marked by ArithFlags

  virtual ValueId ExtendInt  (ValueId first, Type sourceType, Type targetType, ArithFlags flags) = 0;
  virtual ValueId TruncateInt(ValueId first, Type sourceType, Type targetType) = 0;

  // Following are in-future to-be-implemented methods
  //virtual ValueId ConvIntToFloat(ValueId first, uint32_t flags) = 0;
  //virtual ValueId ConvFloatToInt(ValueId first, uint32_t flags) = 0;

  // Creates a new unknown value
  virtual ValueId CreateVal(Type type) = 0;

  virtual ValueId CreateConstIntVal  (uint64_t value, Type type) = 0; 
  // To be potentially removed
  virtual ValueId CreateConstIntVal  (uint64_t value           ) { throw NotImplementedException(); }
  // Floating point support is theoretical right now
  virtual ValueId CreateConstFloatVal(float    value, Type type) { throw NotImplementedException(); }
  // Floating point support is theoretical right now
  virtual ValueId CreateConstFloatVal(double   value, Type type) { throw NotImplementedException(); }

  // Prints current state of the container onto console
  virtual void PrintDebug() { throw NotImplementedException(); }

protected:

  // Zero in all possible interpretations 
  virtual ValueId GetZero() { throw NotImplementedException(); }
  // Zero of specific type
  virtual ValueId GetZero(Type type) const = 0;
};
