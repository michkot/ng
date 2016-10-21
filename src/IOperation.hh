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

  enum class ICmpPredicates {
    ICMP_EQ, ///< equal
    ICMP_NE, ///< not equal
    ICMP_UGT, ///< unsigned greater than
    ICMP_UGE, ///< unsigned greater or equal
    ICMP_ULT, ///< unsigned less than
    ICMP_ULE, ///< unsigned less or equal
    ICMP_SGT, ///< signed greater than
    ICMP_SGE, ///< signed greater or equal
    ICMP_SLT, ///< signed less than
    ICMP_SLE ///< signed less or equal
  };

  enum class FCmpPredicates {
    // Opcode        U L G E    Intuitive operation
    FCMP_FALSE, ///< 0 0 0 0    Always false (always folded)
    FCMP_OEQ,   ///< 0 0 0 1    True if ordered and equal
    FCMP_OGT,   ///< 0 0 1 0    True if ordered and greater than
    FCMP_OGE,   ///< 0 0 1 1    True if ordered and greater than or equal
    FCMP_OLT,   ///< 0 1 0 0    True if ordered and less than
    FCMP_OLE,   ///< 0 1 0 1    True if ordered and less than or equal
    FCMP_ONE,   ///< 0 1 1 0    True if ordered and operands are unequal
    FCMP_ORD,   ///< 0 1 1 1    True if ordered (no nans)
    FCMP_UNO,   ///< 1 0 0 0    True if unordered: isnan(X) | isnan(Y)
    FCMP_UEQ,   ///< 1 0 0 1    True if unordered or equal
    FCMP_UGT,   ///< 1 0 1 0    True if unordered or greater than
    FCMP_UGE,   ///< 1 0 1 1    True if unordered,   greater than,   or equal
    FCMP_ULT,   ///< 1 1 0 0    True if unordered or less than
    FCMP_ULE,   ///< 1 1 0 1    True if unordered,   less than,   or equal
    FCMP_UNE,   ///< 1 1 1 0    True if unordered or not equal
    FCMP_TRUE,  ///< 1 1 1 1    Always true (always folded)
  };
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
  virtual IOperation& UDiv() = 0;
  virtual IOperation& SDiv() = 0;
  virtual IOperation& URem() = 0;
  virtual IOperation& SRem() = 0;

  // Bitwise binary operations
  virtual IOperation& Shl() = 0;
  virtual IOperation& LShr() = 0;
  virtual IOperation& AShr() = 0;
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
  virtual IOperation& ICmp(IOperation::ICmpPredicates pred) = 0;
  virtual IOperation& FCmp(IOperation::FCmpPredicates pred) = 0;

  // Not a supported instruction
  virtual IOperation& NotSupportedInstr() = 0;

  /**
  * Destructor.
  */
  virtual ~IOperationFactory() {}
};