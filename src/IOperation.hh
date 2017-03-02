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
/** @file IOperation.hh */

#pragma once

#include "Definitions.hh"
#include "General.hh"
#include "IState.hh"
#include "StateStorage.hh"

using namespace ::std;

class IOperation {
public:
  //vektor kontext po provedeni, pristi instrukce
  virtual void Execute(IState& originalState, const vector<OperArg>& args) = 0;
};

class OperationNotSupportedOperation : public IOperation {
public:
  virtual void Execute(IState& originalState, const vector<OperArg>& args) override 
  {
    throw NotSupportedException();
  }
};

ENUM_FLAGS(CastOpsEnum)
enum class CastOpsEnum {
  Default       = 0x0000,
  Truncate      = 0x0001, // Right now, both integer and floating point, might change in time
  Extend        = 0x0002,
  FpToInt       = 0x0003,
  IntToFp       = 0x0004,
  PtrToInt      = 0x0005,
  IntToPtr      = 0x0006,
  BitCast       = 0x0007, // same size, different meaning (vectors -> integers, pointers -> pointers
  AddrSpaceCast = 0x0008,
};

//not a true factory, shares one instance
//might be chaned to uptr<..> then it would be true factory
class IOperationFactory {
public:
  // Terminator operations
  virtual IOperation& Ret() = 0;
  virtual IOperation& Br() = 0;

  // Call instructions
  virtual IOperation& Call() = 0;
  virtual IOperation& Invoke() = 0;

  // Binary integers operations
  virtual IOperation& Add() = 0;
  virtual IOperation& Sub() = 0;
  virtual IOperation& Mul() = 0;
  virtual IOperation& Div() = 0;
  virtual IOperation& Rem() = 0;

  // Bitwise binary operations
  virtual IOperation& Shl() = 0;
  virtual IOperation& Shr() = 0;
  virtual IOperation& And() = 0;
  virtual IOperation& Or() = 0;
  virtual IOperation& Xor() = 0;

  // Conversion operations
  virtual IOperation& Cast() = 0;

  // Memory access operations
  virtual IOperation& Alloca() = 0;
  virtual IOperation& Load() = 0;
  virtual IOperation& Store() = 0;
  virtual IOperation& GetElementPtr() = 0;

  // Other operations
  //llvm::CmpInst::Predicate
  virtual IOperation& Cmp() = 0;

  virtual IOperation& Memset() = 0;
  virtual IOperation& Memcpy() = 0;
  virtual IOperation& Memmove() = 0;
  virtual IOperation& Malloc() = 0;
  virtual IOperation& Free() = 0;

  // Not a supported instruction
  virtual IOperation& NotSupportedInstr() = 0;

  /**
  * Destructor.
  */
  virtual ~IOperationFactory() {}
};