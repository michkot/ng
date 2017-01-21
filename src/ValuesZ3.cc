#include "ValuesZ3.hh"

#include <z3++.h>

using namespace z3;

z3::context                 Z3ValueContainer::c;
std::map<ValueId, z3::expr> Z3ValueContainer::idsToExprs;

boost::tribool Z3ResultToTribool(z3::check_result result)
{
  switch (result)
  {
  case z3::unsat:
    return boost::tribool::false_value;
    break;
  case z3::sat:
    return boost::tribool::true_value;
    break;
  case z3::unknown:
    return boost::tribool::indeterminate_value;
    break;
  }
}

void Z3ValueContainer::AddValueInfoToSolver(z3::solver& s, ValueId val) const
{
  // add value's AST
  s.add(idsToExprs.at(val));
  // add all constraints
  for (
    auto it = assumptions.find(val), end = assumptions.cend();
    it != end;
    ++it)
  {
    if (it->first != val)
      break;
    s.add(it->second);
  }
}

z3::expr Z3ValueContainer::CreateCmpExpression(ValueId first, ValueId second, Type type, CmpFlags flags) const
{
  const auto& a = idsToExprs.at(first);
  const auto& b = idsToExprs.at(second);

  // check that the size I assume the value/expression have
  // psedo-code: sizeof(first) == sizeof(second) == type.size
  assert(a.get_sort().bv_size() == b.get_sort().bv_size());
  assert(a.get_sort().bv_size() == type.BitWidth());

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
    throw exception("spatna hodnota"); //TODO: create new exception class?
    break;
  default:
    throw NotSupportedException();
    break;
  }
  c.check_error();
  return expr{c, r};
}



boost::tribool Z3ValueContainer::IsCmp(ValueId first, ValueId second, Type type, CmpFlags flags) const
{
  solver s{c};

  // add value's info / expression (AST, constraints)
  AddValueInfoToSolver(s, first);
  AddValueInfoToSolver(s, second);

  // Cmp
  s.add(CreateCmpExpression(first, second, type, flags));

  // get result
  return Z3ResultToTribool(s.check());
}

boost::tribool Z3ValueContainer::IsEq(ValueId first, ValueId second, Type type) const
{
  return IsCmp(first, second, type, CmpFlags::Eq);
}

boost::tribool Z3ValueContainer::IsNeq(ValueId first, ValueId second, Type type) const
{
  return IsCmp(first, second, type, CmpFlags::Neq);
}

boost::tribool Z3ValueContainer::IsTrue(ValueId first, Type type) const
{
  solver s{c};
  const auto& a = idsToExprs.at(first);

  // check that the size I assume the value/expression have
  // psedo-code: sizeof(first) == sizeof(second) == type.size
  assert(a.get_sort().bv_size() == type.BitWidth());

  // add value's info / expression (AST, constraints)
  AddValueInfoToSolver(s, first);

  // IsZero opposite
  s.add(a != 0);

  // get result
  return Z3ResultToTribool(s.check());
}

boost::tribool Z3ValueContainer::IsFalse(ValueId first, Type type) const
{
  solver s{c};
  const auto& a = idsToExprs.at(first);

  // check that the size I assume the value/expression have
  // psedo-code: sizeof(first) == sizeof(second) == type.size
  assert(a.get_sort().bv_size() == type.BitWidth());

  // add value's info / expression (AST, constraints)
  AddValueInfoToSolver(s, first);

  // IsZero equivalent
  s.add(a == 0);

  // get result
  return Z3ResultToTribool(s.check());
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

  // add value's info / expression (AST, constraints)
  AddValueInfoToSolver(s, first);

  // IsZero
  s.add(idsToExprs.at(first) == 0);

  // get result
  return Z3ResultToTribool(s.check());
}

ValueId Z3ValueContainer::Cmp(ValueId first, ValueId second, Type type, CmpFlags flags)
{
  auto id = ValueId::GetNextId();
  expr ex = CreateCmpExpression(first, second, type, flags);
  idsToExprs.insert({id, ex});

  return id;
}
void Z3ValueContainer::Assume(ValueId first, ValueId second, Type type, CmpFlags flags)
{
  expr e = CreateCmpExpression(first, second, type, flags);

  assumptions.insert({first, e});
  assumptions.insert({second, e});
}

void Z3ValueContainer::AssumeTrue(ValueId first)
{
  const auto& a = idsToExprs.at(first);
  assumptions.insert({first, a != 0});
}

void Z3ValueContainer::AssumeFalse(ValueId first)
{
  const auto& a = idsToExprs.at(first);
  assumptions.insert({first, a == 0});
}

ValueId Z3ValueContainer::Add(ValueId first, ValueId second, Type type, ArithFlags flags)
{
  auto id = ValueId::GetNextId();
  const auto& a = idsToExprs.at(first);
  const auto& b = idsToExprs.at(second);

  // check that the size I assume the value/expression have
  // psedo-code: sizeof(first) == sizeof(second) == type.size
  assert(a.get_sort().bv_size() == b.get_sort().bv_size());
  assert(a.get_sort().bv_size() == type.BitWidth());

  expr ex = a + b;
  idsToExprs.insert({id, ex});
  //TODO: dále podle NSW a NUW (a signed) flagů je třeba přidat ještě no_under* a no_over* predikaty

  return id;
}

ValueId Z3ValueContainer::Sub(ValueId first, ValueId second, Type type, ArithFlags flags)
{
  return CreateVal(type);
}

ValueId Z3ValueContainer::Mul(ValueId first, ValueId second, Type type, ArithFlags flags)
{
  return CreateVal(type);
}

ValueId Z3ValueContainer::Div(ValueId first, ValueId second, Type type, ArithFlags flags)
{
  return CreateVal(type);
}

ValueId Z3ValueContainer::Rem(ValueId first, ValueId second, Type type, ArithFlags flags)
{
  return CreateVal(type);
}

ValueId Z3ValueContainer::LSh(ValueId first, ValueId second, Type type, ArithFlags flags)
{
  return CreateVal(type);
}

ValueId Z3ValueContainer::RSh(ValueId first, ValueId second, Type type, ArithFlags flags)
{
  return CreateVal(type);
}

ValueId Z3ValueContainer::LogAnd(ValueId first, ValueId second, Type type)
{
  return CreateVal(type);
}

ValueId Z3ValueContainer::LogOr(ValueId first, ValueId second, Type type)
{
  return CreateVal(type);
}

ValueId Z3ValueContainer::LogNot(ValueId first, Type type)
{
  return CreateVal(type);
}

ValueId Z3ValueContainer::BitAnd(ValueId first, ValueId second, Type type)
{
  return CreateVal(type);
}

ValueId Z3ValueContainer::BitOr(ValueId first, ValueId second, Type type)
{
  return CreateVal(type);
}

ValueId Z3ValueContainer::BitXor(ValueId first, ValueId second, Type type)
{
  return CreateVal(type);
}

ValueId Z3ValueContainer::BitNot(ValueId first, Type type)
{
  return CreateVal(type);
}

ValueId Z3ValueContainer::ExtendInt(ValueId first, Type sourceType, Type targetType, ArithFlags flags)
{
  auto id = ValueId::GetNextId();
  const auto& a = idsToExprs.at(first);

  assert(a.get_sort().bv_size() == sourceType.BitWidth());

  expr ex = has_flag(flags, ArithFlags::Signed) ?
    sext(a, targetType.BitWidth()) :
    zext(a, targetType.BitWidth());
  idsToExprs.insert({id, ex});

  return id;
}

ValueId Z3ValueContainer::TruncInt(ValueId first, Type sourceType, Type targetType)
{
  auto id = ValueId::GetNextId();
  const auto& a = idsToExprs.at(first);

  assert(a.get_sort().bv_size() == sourceType.BitWidth());

  expr ex = a.extract(targetType.BitWidth(), 0);
  idsToExprs.insert({id, ex});

  return id;
}

ValueId Z3ValueContainer::CreateVal(Type type)
{
  auto id = ValueId::GetNextId();
  auto ex = c.constant(c.int_symbol(static_cast<uint64_t>(id)), c.bv_sort(type.BitWidth()));
  idsToExprs.insert({id, ex});

  return id;

#if 0
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
#endif
}

ValueId Z3ValueContainer::CreateConstIntVal(uint64_t value, Type type)
{
  auto id = ValueId::GetNextId();
  auto ex = c.bv_val(value, type.BitWidth());
  idsToExprs.insert({id, ex});

  return id;
}

ValueId Z3ValueContainer::CreateConstIntVal(uint64_t value)
{
  auto id = ValueId::GetNextId();
  auto ex = c.bv_val(value, 64u);
  idsToExprs.insert({id, ex});

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

void Z3ValueContainer::PrintDebug()
{
  for (auto& pair : idsToExprs)
  {
    printf("\n%d ", pair.first);
    std::cout << pair.second;
  }
}
