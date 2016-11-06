#pragma once

#include "Definitions.hh"
#include "General.hh"
#include "IState.hh"
#include "StateStorage.hh"

using namespace ::std;

class IOperation {
public:
  //vektor kontext po provedeni, pristi instrukce
  virtual void Execute(IState& s, const vector<OperArg>& args) = 0;
  //virtual bool IsConditionalBranch();
};

//not a true factory, shares one instance
//might be chaned to uptr<..> then it would be true factory
class IOperationFactory {
public:
  // Terminator operations
  virtual IOperation& Ret() = 0;
  virtual IOperation& Br() = 0;

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

  // Memory access operations
  virtual IOperation& Alloca() = 0;
  virtual IOperation& Load() = 0;
  virtual IOperation& Store() = 0;
  virtual IOperation& GetElementPtr() = 0;

  // Other operations
  //llvm::CmpInst::Predicate
  virtual IOperation& Cmp() = 0;

  // Not a supported instruction
  virtual IOperation& NotSupportedInstr() = 0;

  /**
  * Destructor.
  */
  virtual ~IOperationFactory() {}
};