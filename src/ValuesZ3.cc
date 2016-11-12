#include "ValuesZ3.hh"

#include <z3++.h>

using namespace z3;

z3::context                 Z3ValueContainer::c;
std::map<ValueId, z3::expr> Z3ValueContainer::idsToExprs;

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
  solver s{c};

  //add value's AST
  s.add(idsToExprs[first]);
  //add all constraints
  for (
    auto it = assumptions.find(first), end = assumptions.cend();
    it != end;
    ++it)
  {
    if (it->first != first)
      break;
    s.add(it->second);
  }

}

ValueId Z3ValueContainer::Cmp(ValueId first, ValueId second, Type type, CmpFlags flags)
{
  auto a = idsToExprs[first];
  auto b = idsToExprs[second];
  check_context(a, b);
  Z3_ast r;
  switch (flags)
  {
  case CmpFlags::Eq:
    r = Z3_mk_eq(c, a, b);
    break;
  case CmpFlags::Neq:
  {
    Z3_ast args[2] = {a, b};
    r = Z3_mk_distinct(c, 2, args);
    break;
  }
  case CmpFlags::UnsigGt:
    r = Z3_mk_bvugt(c, a, b);
    break;
  case CmpFlags::UnsigGtEq:
    r = Z3_mk_bvugt(c, a, b);
    break;
  case CmpFlags::UnsigLt:
    r = Z3_mk_bvugt(c, a, b);
    break;
  case CmpFlags::UnsigLtEq:
    r = Z3_mk_bvugt(c, a, b);
    break;
  case CmpFlags::SigGt:
    r = Z3_mk_bvsgt(c, a, b);
    break;
  case CmpFlags::SigGtEq:
    r = Z3_mk_bvsgt(c, a, b);
    break;
  case CmpFlags::SigLt:
    r = Z3_mk_bvsgt(c, a, b);
    break;
  case CmpFlags::SigLtEq:
    r = Z3_mk_bvsgt(c, a, b);
    break;
  case CmpFlags::Default:
    throw exception("spatna hodnota");
    break;
  default:
    throw NotSupportedException();
    break;
  }
  c.check_error();
  expr e{c, r};

  //? incomplete code!
  //? ! this code is different for both methods
  assumptions.insert({first, e});
  assumptions.insert({second, e});


  auto id = ValueId::GetNextId();

}

void Z3ValueContainer::Assume(ValueId first, ValueId second, Type type, CmpFlags flags)
{

  return;
  // Based on >= operator :
#if 0
  check_context(a, b);
  Z3_ast r = 0;
  if (a.is_arith() && b.is_arith()) {
    r = Z3_mk_ge(a.ctx(), a, b);
  }
  else if (a.is_bv() && b.is_bv()) {
    r = Z3_mk_bvsge(a.ctx(), a, b);
  }
  else {
    // operator is not supported by given arguments.
    assert(false);
  }
  a.check_error();
  return expr(a.ctx(), r);
#endif

}

void Z3ValueContainer::AssumeTrue(ValueId first)
{
  throw NotImplementedException();
}

void Z3ValueContainer::AssumeFalse(ValueId first)
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

ValueId Z3ValueContainer::LogAnd(ValueId first, ValueId second, Type type)
{
 throw NotImplementedException();
}

ValueId Z3ValueContainer::LogOr(ValueId first, ValueId second, Type type)
{
 throw NotImplementedException();
}

ValueId Z3ValueContainer::LogNot(ValueId first, Type type)
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
  auto id = ValueId::GetNextId();
  auto handle = c.constant(c.int_symbol(0), c.bv_sort(type.BitWidth()));
  idsToExprs.insert({id, handle});
  return id;

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
