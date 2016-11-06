#include "Values.hh"

boost::tribool ValueContainer::IsCmp(ValueId first, ValueId second, Type type, CmpFlags flags) const
{
  throw NotImplementedException();
}

boost::tribool ValueContainer::IsCmp(ValueId first, ValueId second, Type type) const
{
  throw NotImplementedException();
}

boost::tribool ValueContainer::IsEq(ValueId first, ValueId second, Type type) const
{
  throw NotImplementedException();
}

boost::tribool ValueContainer::IsNeq(ValueId first, ValueId second, Type type) const
{
  throw NotImplementedException();
}

boost::tribool ValueContainer::IsTrue(ValueId first, Type type) const
{
  throw NotImplementedException();
}

boost::tribool ValueContainer::IsFalse(ValueId first, Type type) const
{
  throw NotImplementedException();
}

boost::tribool ValueContainer::IsBinaryEq(ValueId first, ValueId second) const
{
  throw NotImplementedException();
}

boost::tribool ValueContainer::IsUnknown(ValueId first) const
{
  throw NotImplementedException();
}

boost::tribool ValueContainer::IsZero(ValueId first) const
{
  throw NotImplementedException();
}

ValueId ValueContainer::SetCmp(ValueId first, ValueId second, Type type, CmpFlags flags)
{
  throw NotImplementedException();
}

ValueId ValueContainer::Add(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId ValueContainer::Sub(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId ValueContainer::Mul(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId ValueContainer::Div(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId ValueContainer::Rem(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId ValueContainer::LSh(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId ValueContainer::RSh(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId ValueContainer::LogAnd(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId ValueContainer::LogOr(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId ValueContainer::LogXor(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId ValueContainer::LogNot(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId ValueContainer::BitAnd(ValueId first, ValueId second, Type type)
{
 throw NotImplementedException();
}

ValueId ValueContainer::BitOr(ValueId first, ValueId second, Type type)
{
 throw NotImplementedException();
}

ValueId ValueContainer::BitXor(ValueId first, ValueId second, Type type)
{
 throw NotImplementedException();
}

ValueId ValueContainer::BitNot(ValueId first, Type type)
{
 throw NotImplementedException();
}

ValueId ValueContainer::ConvIntToFloat(ValueId first, uint32_t flags)
{
 throw NotImplementedException();
}

ValueId ValueContainer::ConvFloatToInt(ValueId first, uint32_t flags)
{
 throw NotImplementedException();
}

ValueId ValueContainer::CreateVal()
{
  return ValueId();
}

ValueId ValueContainer::CreateConstIntVal(uint64_t value, Type type)
{
 throw NotImplementedException();
}

ValueId ValueContainer::CreateConstIntVal(int64_t value, Type type)
{
  return ValueId();
}

ValueId ValueContainer::CreateConstFloatVal(float value, Type type)
{
  return ValueId();
}

ValueId ValueContainer::CreateConstFloatVal(double value, Type type)
{
  return ValueId();
}