/*******************************************************************************

Copyright (C) 2017 Michal Kotoun

This file is a part of Angie project.

Angie is free software: you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option)
any later version.

Angie is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with Angie.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/
/** @file General.hh */

#pragma once

#include <cstdint>

#include "Type.hh"
#include "enum_flags.h"


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

enum class BinaryOpKind : int16_t {
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

enum class CastOpKind : int16_t {
  Default = 0x0000,
  Truncate = 0x0001, // Right now, both integer and floating point, might change in time
  Extend = 0x0002,
  FpToInt = 0x0003,
  IntToFp = 0x0004,
  PtrToInt = 0x0005,
  IntToPtr = 0x0006,
  BitCast = 0x0007, // same size, different meaning (vectors -> integers, pointers -> pointers
  AddrSpaceCast = 0x0008,
};

ENUM_FLAGS_TYPED(ArithFlags, int16_t)
enum class ArithFlags : int16_t {
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

ENUM_FLAGS_TYPED(CmpFlags, int16_t)
enum class CmpFlags : int16_t {
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

struct BinaryOpOptions {
  BinaryOpKind opKind;
  ArithFlags   flags;
};

struct CastOpOptions {
  CastOpKind  opKind;
  ArithFlags   flags;
};

struct FrontendIdTypePair{
public:
  FrontendValueId id;
  Type type;  

  /*ctr*/ FrontendIdTypePair(FrontendValueId id, Type type) : id{id}, type{type} {}
  operator std::pair<FrontendValueId, Type>() { return {id, type}; }
};

struct OperArg {
public:
  union {
    struct {
      [[deprecated]] FrontendValueId id; // DEPRECATED
      [[deprecated]] Type type; // DEPRECATED
    };
    FrontendIdTypePair idTypePair;
    BinaryOpOptions binOpOpts;
    CastOpOptions   castOpOpts;
    ArithFlags      arithFlags;
    CmpFlags        cmpFlags;
  };
private:
  /*ctr*/ OperArg() : idTypePair{FrontendValueId{0}, Type{0}} {}
public:
  /*ctr*/ OperArg(FrontendValueId id, Type type) : idTypePair{id, type} {}
  /*ctr*/ OperArg(BinaryOpKind opKind, ArithFlags flags) : binOpOpts{opKind, flags} {}
  /*ctr*/ OperArg(CastOpKind opKind, ArithFlags flags) : castOpOpts{opKind, flags} {}
  /*ctr*/ OperArg(ArithFlags flags) : arithFlags{flags} {}
  /*ctr*/ OperArg(CmpFlags flags) : cmpFlags{flags} {}
  static OperArg CreateEmptyArg() { return OperArg{}; };
};

class OperationArgs {
protected:
  std::vector<OperArg> args;
public:

  std::vector<OperArg>& GetArgs() { return args; };
  OperArg GetTarget() { return args[0]; }
  OperArg GetOptions() { return args[1]; }
};

// Structrure of the created vector:
// 0: return value | empty arg if it its void-type
// 1: function call target | operation's cmp or arithmetic flags | empty 
// 2+: operands

class CastOperationArgs : public OperationArgs {
public:
  CastOpOptions GetOptions()   { args.at(1).castOpOpts; }
};

class BinaryOpArgs : public OperationArgs {
public:
  BinaryOpOptions GetOptions() { args.at(1).binOpOpts; }
};

//class CallOpArgs : public OperationArgs {
//public:
//  BinaryOpOptions GetOptions() { args[1].binOpOptions; }
//};
