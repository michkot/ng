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