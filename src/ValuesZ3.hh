#pragma once

#include "Definitions.hh"
#include "Type.hh"

#include "Values.hh"

#include <boost/logic/tribool.hpp>

class Z3ValueContainer : public IValueContainer {
public:
  // parametrzied
  virtual boost::tribool IsCmp(ValueId first, ValueId second, Type type, CmpFlags flags) const override;
  // signed "C style" by default
  virtual virtual boost::tribool IsCmp     (ValueId first, ValueId second, Type type) const override;
  virtual boost::tribool IsEq      (ValueId first, ValueId second, Type type) const override;
  virtual boost::tribool IsNeq     (ValueId first, ValueId second, Type type) const override;
  virtual boost::tribool IsTrue    (ValueId first, Type type) const override;
  virtual boost::tribool IsFalse   (ValueId first, Type type) const override;

  virtual boost::tribool IsBinaryEq(ValueId first, ValueId second) const override;
  virtual boost::tribool IsUnknown (ValueId first) const override;
  virtual boost::tribool IsZero    (ValueId first) const override;

  virtual ValueId SetCmp(ValueId first, ValueId second, Type type, CmpFlags flags) override;

  virtual ValueId Add   (ValueId first, ValueId second, Type type, ArithFlags flags) override;
  virtual ValueId Sub   (ValueId first, ValueId second, Type type, ArithFlags flags) override;
  virtual ValueId Mul   (ValueId first, ValueId second, Type type, ArithFlags flags) override;
  virtual ValueId Div   (ValueId first, ValueId second, Type type, ArithFlags flags) override;
  virtual ValueId Rem   (ValueId first, ValueId second, Type type, ArithFlags flags) override;

  virtual ValueId LSh   (ValueId first, ValueId second, Type type, ArithFlags flags) override;
  virtual ValueId RSh   (ValueId first, ValueId second, Type type, ArithFlags flags) override;

  virtual ValueId LogAnd(ValueId first, ValueId second, Type type, ArithFlags flags) override;
  virtual ValueId LogOr (ValueId first, ValueId second, Type type, ArithFlags flags) override;
  virtual ValueId LogXor(ValueId first, ValueId second, Type type, ArithFlags flags) override;
  virtual ValueId LogNot(ValueId first, ValueId second, Type type, ArithFlags flags) override;

  virtual ValueId BitAnd(ValueId first, ValueId second, Type type) override;
  virtual ValueId BitOr (ValueId first, ValueId second, Type type) override;
  virtual ValueId BitXor(ValueId first, ValueId second, Type type) override;
  virtual ValueId BitNot(ValueId first, Type type) override;

  virtual ValueId ConvIntToFloat(ValueId first, uint32_t flags) override;
  virtual ValueId ConvFloatToInt(ValueId first, uint32_t flags) override;

  virtual ValueId CreateVal() override;

  virtual ValueId CreateConstIntVal  (uint64_t value, Type type) override;
  //TODO: Maybe remove, because value should be binary representation
  virtual ValueId CreateConstIntVal  (uint64_t value           ) override;
  virtual ValueId CreateConstFloatVal(float    value, Type type) override;
  virtual ValueId CreateConstFloatVal(double   value, Type type) override;
};