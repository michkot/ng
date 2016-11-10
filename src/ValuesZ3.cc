#include "ValuesZ3.hh"

#include <z3++.h>

using namespace z3;

boost::tribool Z3ValueContainer::IsCmp(ValueId first, ValueId second, Type type, CmpFlags flags) const
{
  throw NotImplementedException();
}

boost::tribool Z3ValueContainer::IsEq(ValueId first, ValueId second, Type type) const
{
  throw NotImplementedException();
}

boost::tribool Z3ValueContainer::IsNeq(ValueId first, ValueId second, Type type) const
{
  throw NotImplementedException();
}

boost::tribool Z3ValueContainer::IsTrue(ValueId first, Type type) const
{
  throw NotImplementedException();
}

boost::tribool Z3ValueContainer::IsFalse(ValueId first, Type type) const
{
  throw NotImplementedException();
}

boost::tribool Z3ValueContainer::IsBinaryEq(ValueId first, ValueId second) const
{
  throw NotImplementedException();
}

boost::tribool Z3ValueContainer::IsUnknown(ValueId first) const
{
  throw NotImplementedException();
}

boost::tribool Z3ValueContainer::IsZero(ValueId first) const
{
  throw NotImplementedException();
}

ValueId Z3ValueContainer::Assume(ValueId first, ValueId second, Type type, CmpFlags flags)
{
  throw NotImplementedException();
}

ValueId Z3ValueContainer::Add(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId Z3ValueContainer::Sub(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId Z3ValueContainer::Mul(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId Z3ValueContainer::Div(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId Z3ValueContainer::Rem(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId Z3ValueContainer::LSh(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId Z3ValueContainer::RSh(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId Z3ValueContainer::LogAnd(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId Z3ValueContainer::LogOr(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId Z3ValueContainer::LogXor(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId Z3ValueContainer::LogNot(ValueId first, ValueId second, Type type, ArithFlags flags)
{
 throw NotImplementedException();
}

ValueId Z3ValueContainer::BitAnd(ValueId first, ValueId second, Type type)
{
 throw NotImplementedException();
}

ValueId Z3ValueContainer::BitOr(ValueId first, ValueId second, Type type)
{
 throw NotImplementedException();
}

ValueId Z3ValueContainer::BitXor(ValueId first, ValueId second, Type type)
{
 throw NotImplementedException();
}

ValueId Z3ValueContainer::BitNot(ValueId first, Type type)
{
  throw NotImplementedException();
}

ValueId Z3ValueContainer::ExtendInt(ValueId first, Type sourceType, Type targetType)
{
  throw NotImplementedException();
}

ValueId Z3ValueContainer::TruncInt(ValueId first, Type sourceType, Type targetType)
{
  throw NotImplementedException();
}

ValueId Z3ValueContainer::CreateVal(Type type)
{
  //c.constant(c.int_symbol(static_cast<uint64_t>(id)), c.bv_sort(64));
  try {
    std::cout << "find_model_example1\n";
    context c;

    expr x = c.constant(c.int_symbol(0), c.bv_sort(64)); //type.BitWidth()
    expr y = c.constant(c.int_symbol(1), c.bv_sort(64));
    expr vyraz1 = x >= 1;
    expr vyraz2 = y < x + 3;

    solver s(c);

    s.add(vyraz1);
    s.add(vyraz2);
    std::cout << s.check() << "\n";

    model m = s.get_model();
    std::cout << m << "\n";
    // traversing the model
    for (unsigned i = 0; i < m.size(); i++) {
      func_decl v = m[i];
      // this problem contains only constants
      assert(v.arity() == 0);
      std::cout << v.name() << " = " << m.get_const_interp(v) << "\n";
    }
    // we can evaluate expressions in the model.
    std::cout << "x + y + 1 = " << m.eval(x + y + 1) << "\n";
  }
  catch (exception & ex) {
    std::cout << "unexpected error: " << ex << "\n";
  }
  return ValueId::GetNextId();
}

ValueId Z3ValueContainer::CreateConstIntVal(uint64_t value, Type type)
{
 throw NotImplementedException();
}

ValueId Z3ValueContainer::CreateConstIntVal(uint64_t value)
{
  auto id = ValueId::GetNextId();
  context c;

  auto handle = c.constant(c.int_symbol(static_cast<uint64_t>(id)), c.bv_sort(64u));
  auto val = c.bv_val(value, 64u);

  solver s(c);
  s.add(handle == val);
  return id;
}

ValueId Z3ValueContainer::CreateConstFloatVal(float value, Type type)
{
  throw NotImplementedException();
}

ValueId Z3ValueContainer::CreateConstFloatVal(double value, Type type)
{
  throw NotImplementedException();
}
