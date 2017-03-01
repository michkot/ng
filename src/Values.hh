#pragma once

#include "Definitions.hh"
#include "Type.hh"
#include "IdImpl.hh"

#include "enum_flags.h"

#include <boost/logic/tribool.hpp>

class ValueIdToken;
/// <summary>
/// ValueId class is an type-safe encapsulation of integer-based IDs for values
/// </summary>
using ValueId = Id<ValueIdToken>;

#include "enum_flags.h"
ENUM_FLAGS(BinaryOpKind)
enum class BinaryOpKind {
  Default  = 0x0000, // Is an error

  Add      = 0x0001, // Standard two's complement operation
  Sub      = 0x0002, // Standard two's complement operation
  Mul      = 0x0003, // Standard two's complement operation

  Div      = 0x0004,
  Rem      = 0x0005,

  Shl      = 0x0006, // Bitshift left
  Shr      = 0x0007, // Bitshift right, unsigned(logical) or signed(arithmetic) 

  And      = 0x0008, // Bitwise AND
  Or       = 0x0009, // Bitwise OR
  Xor      = 0x000A, // Bitwise XOR
};

ENUM_FLAGS(ArithFlags)
enum class ArithFlags {
  Default        = 0x0000, // Defaults to "C unsigned behaviour"; Error for div/rem/shr operations
  Signed         = 0x0001, // Needed for div, rem, shr
  Unsigned       = 0x0002, // Needed for div, rem, shr
  Exact          = 0x0004, // Undefined if operation would not return exact result (loss of digits/precision)
  NoSignedWrap   = 0x0008, // Undefined on signed overflow
  NoUnsignedWrap = 0x0010, // Undefined on unsigned overflow
  FmfFlag1       = 0x0020, // Reserved for floats
  FmfFlag2       = 0x0040, // Reserved for floats
  FmfFlag3       = 0x0080, // Reserved for floats
  FmfFlag4       = 0x0100, // Reserved for floats
  FmfFlag5       = 0x0200, // Reserved for floats
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

ENUM_FLAGS(AbstractionStatus)
enum class AbstractionStatus {
  Undefined = 0x0000, // Value is purely abstract and no information were given to the VC
  Unknown = 0x0001, // Value might have had some asociated information but now purely unknown
};

constexpr const char* AbstractionStatusToString(const AbstractionStatus s)
{
  return
    s == AbstractionStatus::Undefined ?
      "Undefined" :
      "Unknown";
}

struct BinaryOpOptions {
  BinaryOpKind opKind;
  ArithFlags   flags;
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

  // General note for implementor of this abstract class / interface:
  // Methods need implementing for compiling:
  // IsCmp, IsInternalRepEq, IsZero, IsUnknown, 
  // Assume, Cmp, BinOp, BitNot, 
  // ExtendInt, TruncateInt, CreateVal, CreateConstIntVal, 
  // GetZero, 
  // PrintDebug
  // Other methods which analysis is probably going to use (and might be changed from NIE to pure virtual):
  // AssumeTrue, AssumeFalse, GetAbstractionStatus

  // --------------------------------------------------------------------------
  // Section A - constant methods, comparison queries
  
  // The first method is an universal method for comparison of two values
  // The others are specialzed helper / convenient methods

  virtual boost::tribool IsCmp  (ValueId first, ValueId second, Type type, CmpFlags flags) const = 0;
  virtual boost::tribool IsEq   (ValueId first, ValueId second, Type type) const 
                                    { return IsCmp(first, second, type, CmpFlags::Eq); }
  virtual boost::tribool IsNeq  (ValueId first, ValueId second, Type type) const 
                                    { return IsCmp(first, second, type, CmpFlags::Neq); }
  virtual boost::tribool IsTrue (ValueId first, Type type) const 
                                    { return IsCmp(first, GetZero(type), type, CmpFlags::Neq); } // !=0
  virtual boost::tribool IsFalse(ValueId first, Type type) const 
                                    { return IsCmp(first, GetZero(type), type, CmpFlags::Eq); }  // ==0
  
  // Compares values' internal representations - typically a bitwise comparison of fixed-length integers
  virtual boost::tribool IsInternalRepEq(ValueId first, ValueId second) const = 0;
  // The value is zero in all possible interpretations 
  virtual boost::tribool IsZero   (ValueId first) const = 0;
  // There are no informations regarding this value
  virtual bool           IsUnknown(ValueId first) const = 0;
  //TODO: relocate, add comment
  virtual AbstractionStatus GetAbstractionStatus(ValueId first) { throw NotImplementedException(); } 

  // --------------------------------------------------------------------------
  // Section B - modifying methods, constraint addition 

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
  // Section C - modifying methods, math/bitwise binary and unary operations

  // In following methods, the representation of result of the operation is assigned to the returned value 
  // (need not be a newly created one, e.g. typically in case of constants)

  // Universal arithmetic/bitwise binary operation, configured by struct param  
  virtual ValueId BinOp (ValueId first, ValueId second, Type type, BinaryOpOptions options) = 0;
  virtual ValueId BinOp (ValueId first, ValueId second, Type type, BinaryOpKind kind, ArithFlags flags)
                            { return BinOp(first, second, type, {kind, flags}); }

  virtual ValueId Add   (ValueId first, ValueId second, Type type, ArithFlags flags) 
                            { return BinOp(first, second, type, {BinaryOpKind::Add, flags}); }
  virtual ValueId Sub   (ValueId first, ValueId second, Type type, ArithFlags flags)
                            { return BinOp(first, second, type, {BinaryOpKind::Sub, flags}); }
  virtual ValueId Mul   (ValueId first, ValueId second, Type type, ArithFlags flags)
                            { return BinOp(first, second, type, {BinaryOpKind::Mul, flags}); }
  virtual ValueId Div   (ValueId first, ValueId second, Type type, ArithFlags flags)
                            { return BinOp(first, second, type, {BinaryOpKind::Div, flags}); }
  virtual ValueId Rem   (ValueId first, ValueId second, Type type, ArithFlags flags)
                            { return BinOp(first, second, type, {BinaryOpKind::Rem, flags}); }

  virtual ValueId ShL   (ValueId first, ValueId second, Type type, ArithFlags flags)
                            { return BinOp(first, second, type, {BinaryOpKind::Shl, flags}); }
  virtual ValueId ShR   (ValueId first, ValueId second, Type type, ArithFlags flags)
                            { return BinOp(first, second, type, {BinaryOpKind::Shr, flags}); }

  virtual ValueId BitAnd(ValueId first, ValueId second, Type type)
                            { return BinOp(first, second, type, {BinaryOpKind::And, ArithFlags::Default}); }
  virtual ValueId BitOr (ValueId first, ValueId second, Type type)
                            { return BinOp(first, second, type, {BinaryOpKind::Or,  ArithFlags::Default}); }
  virtual ValueId BitXor(ValueId first, ValueId second, Type type)
                            { return BinOp(first, second, type, {BinaryOpKind::Xor, ArithFlags::Default}); }
  virtual ValueId BitNot(ValueId first, Type type) = 0; // Only unary operation !!!

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

  // --------------------------------------------------------------------------
  // Section E - diagnostic methods

  // Prints current state of the container onto console
  virtual void PrintDebug() const { throw NotImplementedException(); }
  //virtual void Print(std::ostream& os) const { throw NotImplementedException(); }

protected:

  // Zero in all possible interpretations 
  virtual ValueId GetZero() const { throw NotImplementedException(); }
  // Zero of specific type/size
  virtual ValueId GetZero(Type type) const = 0;
};

//inline std::ostream& operator<<(std::ostream& os, const IValueContainer& vc) { vc.Print(os); return os; }
