#pragma once

#include "Definitions.hh"
#include "Type.hh"

#include "Values.hh"

#include <boost/logic/tribool.hpp>
#include <map>
#include <z3++.h>

//TODO: refactor one-names
//TODO: lhs, rhs

class Z3ValueContainer : public IValueContainer {
private:
  static z3::context c; //TODO: refactor, con? context?
  static std::map<ValueId, z3::expr> idsToExprs;
  std::multimap<ValueId, z3::expr> assumptions; //TODO: contraints?

  z3::expr CreateCmpExpression(ValueId first, ValueId second, Type type, CmpFlags flags) const;
  void     AddValueInfoToSolver(z3::solver& s, ValueId val) const;
public:
  virtual boost::tribool IsCmp     (ValueId first, ValueId second, Type type, CmpFlags flags) const override;
  virtual boost::tribool IsEq      (ValueId first, ValueId second, Type type) const override;
  virtual boost::tribool IsNeq     (ValueId first, ValueId second, Type type) const override;
  virtual boost::tribool IsTrue    (ValueId first, Type type) const override; // !=0
  virtual boost::tribool IsFalse   (ValueId first, Type type) const override; // ==0

  virtual boost::tribool IsInternalRepEq(ValueId first, ValueId second) const override;
  virtual boost::tribool IsZero   (ValueId first) const override;
  virtual bool           IsUnknown(ValueId first) const override;
  // Creates new boolean (1bit integer) value expressing the constraint
  virtual ValueId Cmp        (ValueId first, ValueId second, Type type, CmpFlags flags) override;
  // Sets constraint on both values
  virtual void    Assume     (ValueId first, ValueId second, Type type, CmpFlags flags) override;
  virtual void    AssumeTrue (ValueId first) override; // Sets contraint: first != 0 ( == true )
  virtual void    AssumeFalse(ValueId first) override; // Sets contraint: first == 0 ( == false)

  virtual ValueId Add   (ValueId first, ValueId second, Type type, ArithFlags flags) override;
  virtual ValueId Sub   (ValueId first, ValueId second, Type type, ArithFlags flags) override;
  virtual ValueId Mul   (ValueId first, ValueId second, Type type, ArithFlags flags) override;
  virtual ValueId Div   (ValueId first, ValueId second, Type type, ArithFlags flags) override;
  virtual ValueId Rem   (ValueId first, ValueId second, Type type, ArithFlags flags) override;

  virtual ValueId LSh   (ValueId first, ValueId second, Type type, ArithFlags flags) override;
  virtual ValueId RSh   (ValueId first, ValueId second, Type type, ArithFlags flags) override;

  virtual ValueId BitAnd(ValueId first, ValueId second, Type type) override;
  virtual ValueId BitOr (ValueId first, ValueId second, Type type) override;
  virtual ValueId BitXor(ValueId first, ValueId second, Type type) override;
  virtual ValueId BitNot(ValueId first, Type type) override;

  //following are not an LLVM operations
  //when C/C++ is compiled in LLVM, everything is series of != 0 tests and branch/phi instructions
  virtual ValueId LogAnd(ValueId first, ValueId second, Type type) override;
  virtual ValueId LogOr (ValueId first, ValueId second, Type type) override;
  virtual ValueId LogNot(ValueId first, Type type) override;

  virtual ValueId ExtendInt(ValueId first, Type sourceType, Type targetType, ArithFlags flags) override;
  virtual ValueId TruncateInt (ValueId first, Type sourceType, Type targetType) override;

  //virtual ValueId ConvIntToFloat(ValueId first, uint32_t flags) override;
  //virtual ValueId ConvFloatToInt(ValueId first, uint32_t flags) override;

  virtual ValueId CreateVal(Type type) override;

  virtual ValueId CreateConstIntVal  (uint64_t value, Type type) override;
  virtual ValueId CreateConstIntVal  (uint64_t value           ) override; // To be potentially removed
  virtual ValueId CreateConstFloatVal(float    value, Type type) override;
  virtual ValueId CreateConstFloatVal(double   value, Type type) override;

  virtual void PrintDebug() override;

protected:

  virtual ValueId GetZero(Type type) const override { throw NotImplementedException(); }
};