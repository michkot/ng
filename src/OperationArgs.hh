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
/** @file OperationArgs.hh */

//#pragma once
//
//#include "Definitions.hh"
//#include "General.hh"
//
//class OperationArgs {
//protected:
//  std::vector<OperArg> args;
//public:
//  std::vector<OperArg>& GetArgs() { return args; };
//  OperArg GetTarget() { return args[0]; }
//  OperArg GetOptions() { return args[1]; }
//};
//
//
//enum class CastOpsEnum {
//  Default = 0x0000,
//  Truncate = 0x0001, // Right now, both integer and floating point, might change in time
//  Extend = 0x0002,
//  FpToInt = 0x0003,
//  IntToFp = 0x0004,
//  PtrToInt = 0x0005,
//  IntToPtr = 0x0006,
//  BitCast = 0x0007, // same size, different meaning (vectors -> integers, pointers -> pointers
//  AddrSpaceCast = 0x0008,
//};
//
//class CastOperationArgs : public OperationArgs {
//public:
//  CastOpsEnum GetOptions() { return static_cast<CastOpsEnum>(static_cast<uint64_t>(args[1].id)); }
//};