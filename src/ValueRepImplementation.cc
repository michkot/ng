#include "ValueRepImplementation.hh"

#define CHECK_FLAG(var, flag) ((var & flag) == flag)

ConstraintId ValueContainer::nextConstraintIdToGive = 0;

BinaryConstraint::BinaryConstraint(ValueId _first, ValueId _second, CmpFlags _relation)
{
  if (first > second)
  {
    std::swap(first, second);
    switch(_relation)
    {
    case CmpFlags::Eq:
    case CmpFlags::Neq:
      //missing break intended
      break;

    case CmpFlags::SigGt:
      _relation = CmpFlags::SigLt;
      break;
    case CmpFlags::SigGtEq:
      _relation = CmpFlags::SigLtEq;
      break;
    case CmpFlags::SigLt:
      _relation = CmpFlags::SigGt;
      break;
    case CmpFlags::SigLtEq:
      _relation = CmpFlags::SigGtEq;
      break;
    case CmpFlags::UnsigGt:
      _relation = CmpFlags::UnsigLt;
      break;
    case CmpFlags::UnsigGtEq:
      _relation = CmpFlags::UnsigLtEq;
      break;
    case CmpFlags::UnsigLt:
      _relation = CmpFlags::UnsigGt;
      break;
    case CmpFlags::UnsigLtEq:
      _relation = CmpFlags::UnsigGtEq;
      break;
    default:
      throw std::runtime_error("Creating binaryconstraint with odd relation");
    }
  }

  this->first = _first;
  this->second = _second;
  this->relation = _relation;
}

void ValueContainer::InsertConstraint(BinaryConstraint constr)
{
  //invalidate old constraints
  //--------------------------
  bool isConstraintWithConstant = IsConstant(constr.first) || IsConstant(constr.second);
  if (isConstraintWithConstant)
  {
    //relation with a constant -> update intervals
    if (IsConstant(constr.first))
    {
      //first valueId is a constant
      for (auto oldConstraintId : GetConstraintIdVector(constr.second))
      {
        //for each constraint of nonconstatnt valueId
        auto oldConstraint = constrContainer.find(oldConstraintId);
        if (oldConstraint == constrContainer.end())
          throw std::runtime_error("ConstraintId references constraint that does not exist.");

        switch (constr.relation)
        {
        case CmpFlags::Eq:
          //new constraint is equal to a constant -> delete all previous
          DeleteConstraint(oldConstraintId);
          break;
        //TODO::add cases for intervals
          default:
            break;
        }

      }

    }
    else
    {
      //second valueId is a constant
      for (auto oldConstraintId : GetConstraintIdVector(constr.first))
      {
        //for each constraint of nonconstatnt valueId
        auto oldConstraint = constrContainer.find(oldConstraintId);
        if (oldConstraint == constrContainer.end())
          throw std::runtime_error("ConstraintId references constraint that does not exist.");

        switch (constr.relation)
        {
        case CmpFlags::Eq:
          //new constraint is equal to a constant -> delete all previous
          DeleteConstraint(oldConstraintId);
          break;
          //TODO::add cases for intervals
        default:
          break;
        }

      }

    }
  }
  

  //invalidate constraints with each other
  for (auto oldConstraintId : GetConstraintIdVector(constr.first))
  {
    if (constr.second == constrContainer.at(oldConstraintId).GetOther(constr.first))
    {
      //delete mutual constraints
      DeleteConstraint(oldConstraintId);
    }

  }
  
  //create and insert new constraint
  //--------------------------------
  ConstraintId newConstraintId = GetNextConstraintId();
  constrContainer[newConstraintId] = constr;

  //add constraint to both ValueIds
  constrIdContainer[constr.first].push_back(newConstraintId);
  constrIdContainer[constr.second].push_back(newConstraintId);
}

void ValueContainer::DeleteConstraint(ConstraintId constrId)
{
  auto constraint = constrContainer.find(constrId);
  if (constraint == constrContainer.end())
    throw std::runtime_error("Trying to delete constraint that does not exist");

  auto lhs = constrIdContainer.find(constraint->second.first); //first valueId
  auto rhs = constrIdContainer.find(constraint->second.second); //second valueId

  if (lhs == constrIdContainer.end() || rhs == constrIdContainer.end())
    throw std::runtime_error("ValueId contained in a constraint does not reference the constraint");

  lhs->second.erase(std::remove(lhs->second.begin(), lhs->second.end(), constrId), lhs->second.end());
  rhs->second.erase(std::remove(rhs->second.begin(), rhs->second.end(), constrId), rhs->second.end());

  constrContainer.erase(constrId);
}

const std::vector<ConstraintId>& ValueContainer::GetConstraintIdVector(const ValueId id) const
{
  auto lhsConstrId = constrIdContainer.find(id);

  //is there a record of any constraints for the id
  if (lhsConstrId == constrIdContainer.end())
    return std::vector<ConstraintId>();

  //return reference to the vector of constraintIds corresponding to the id
  return lhsConstrId->second;
}


boost::tribool ValueContainer::IsCmp(ValueId first, ValueId second, Type type, CmpFlags flags) const
{
  //NOTE::first and second must be sorted

  if (IsConstant(first) && IsConstant(second))
  {
    //both are constants
    int64_t val1 = (int64_t)SignExtend64(type.BitWidth(), constantContainer.at(first));
    int64_t val2 = (int64_t)SignExtend64(type.BitWidth(), constantContainer.at(second));
    uint64_t uval1 = RipBits(type.BitWidth(), constantContainer.at(first));
    uint64_t uval2 = RipBits(type.BitWidth(), constantContainer.at(second));

    switch (flags)
    {
    case CmpFlags::Eq:
      return uval1 == uval2;
    case CmpFlags::Neq:
      return uval1 != uval2;
    case CmpFlags::SigGt:
      return  val1 > val2;
    case CmpFlags::SigGtEq:
      return  val1 >= val2;
    case CmpFlags::SigLt:
      return  val1 < val2;
    case CmpFlags::SigLtEq:
      return  val1 <= val2;
    case CmpFlags::UnsigGt:
      return  uval1 > uval2;
    case CmpFlags::UnsigGtEq:
      return  uval1 >= uval2;
    case CmpFlags::UnsigLt:
      return  uval1 < uval2;
    case CmpFlags::UnsigLtEq:
      return  uval1 <= uval2;
    default:
      throw std::runtime_error("Unexpected binary constraint");
    }

  }


  else if (IsConstant(first) || IsConstant(second))
  {
    //only one of ValueIds is a constant

    if (IsConstant(second))
      std::swap(first, second);

    //first is a constant
    int64_t val1 = (int64_t)SignExtend64(type.BitWidth(), constantContainer.at(first));
    uint64_t uval1 = RipBits(type.BitWidth(), constantContainer.at(first));

    //go through constraints of second ValueId and check for constraints with other constants
    for (auto constraintId : GetConstraintIdVector(second))
    {
      auto constraint = constrContainer.at(constraintId);
      ValueId other = constraint.GetOther(second);
      if (IsConstant(other))
      {
        //constaint with a constant
        int64_t val2 = (int64_t)SignExtend64(type.BitWidth(), constantContainer.at(other));
        uint64_t uval2 = RipBits(type.BitWidth(), constantContainer.at(other));

        switch (flags)
        {
        case CmpFlags::Eq:
          if (constraint.relation == CmpFlags::Eq)
            return uval1 == uval2;
          if (constraint.relation == CmpFlags::Neq && uval1 == uval2)
            return false;
          //add intervals -- needs even more layers first<>second in constraint...
          break;
        case CmpFlags::Neq:
          if (constraint.relation == CmpFlags::Eq)
            return uval1 != uval2;
          if (constraint.relation == CmpFlags::Neq && uval1 == uval2)
            return true;
          //intervals require more layers
        case CmpFlags::Gt:
        case CmpFlags::GtEq:
        case CmpFlags::Lt:
        case CmpFlags::LtEq:
        default:
          break;
        }

      }

    }
  }
  else
  {
    //neither are a constant

    //loop through constraintIds corresponding to the first ValueId
    for(auto constrId : GetConstraintIdVector(first))
    { 
      auto constraint = constrContainer.find(constrId);
      if (constraint == constrContainer.end())
        throw std::runtime_error("ConstraintId references constraint that does not exist.");
      if (second == constraint->second.GetOther(first))
      {
        //this constraint contains both valueIds
        switch (flags)
        {
        case CmpFlags::Eq:
          if (constraint->second.relation == CmpFlags::Eq)
            return true;
          if (constraint->second.relation == CmpFlags::Neq || constraint->second.relation == CmpFlags::Gt || constraint->second.relation == CmpFlags::Lt)
            return false;
          break;
        case CmpFlags::Neq:
          if (constraint->second.relation == CmpFlags::Eq)
            return false;
          if (constraint->second.relation == CmpFlags::Neq || constraint->second.relation == CmpFlags::Gt || constraint->second.relation == CmpFlags::Lt)
            return true;
          break;
        case CmpFlags::Gt:
          if (constraint->second.relation == CmpFlags::Gt)
            return true;
          if (constraint->second.relation == CmpFlags::Eq || constraint->second.relation == CmpFlags::Lt || constraint->second.relation == CmpFlags::LtEq)
            return false;
          break;
        case CmpFlags::Lt:
          if (constraint->second.relation == CmpFlags::Lt)
            return true;
          if (constraint->second.relation == CmpFlags::Eq || constraint->second.relation == CmpFlags::Gt || constraint->second.relation == CmpFlags::GtEq)
            return false;
          break;
        case CmpFlags::GtEq:
          if (constraint->second.relation == CmpFlags::Gt || constraint->second.relation == CmpFlags::GtEq || constraint->second.relation == CmpFlags::Eq)
            return true;
          if (constraint->second.relation == CmpFlags::Lt)
            return false;
          break;
        case CmpFlags::LtEq:
          if (constraint->second.relation == CmpFlags::Lt || constraint->second.relation == CmpFlags::LtEq || constraint->second.relation == CmpFlags::Eq)
            return true;
          if (constraint->second.relation == CmpFlags::Gt)
            return false;
          break;
        default:
          break;
        }
      }	
    }

  }

  return boost::indeterminate;
}

boost::tribool ValueContainer::IsEq(ValueId first, ValueId second, Type type) const
{
  if (first == second)
    return true;

  //if both constants but different
  if (IsConstant(first) && IsConstant(second))
    return RipBits(type.BitWidth(), constantContainer.at(first)) == RipBits(type.BitWidth(), constantContainer.at(second));

  //check for equality in constraint container
  //------------------------------------------
  if (first > second)
    std::swap(first, second);


  //loop through vector of constraintIds of corresponding ValueId
  for (auto constrId : GetConstraintIdVector(first))
  {
    auto constr = constrContainer.find(constrId);
    if (constr == constrContainer.end())
      throw std::runtime_error("Referenced constraint does not exist!");

    //check direct relation
    if (constr->second.Contains(second))
    {
      switch (constr->second.relation)
      {
      case CmpFlags::Eq:
        return true;

      case CmpFlags::Neq:
      case CmpFlags::SigGt:
      case CmpFlags::SigLt:
      case CmpFlags::UnsigGt:
      case CmpFlags::UnsigLt:
        return false;

      default:
        break;
      }
    }

    //TODO::check intervals
  }


  return boost::indeterminate;
}

boost::tribool ValueContainer::IsNeq(ValueId first, ValueId second, Type type) const //maybe (!IsEq)?
{
  return !IsEq(first, second, type);
}

boost::tribool ValueContainer::IsTrue(ValueId first, Type type) const
{
  //is a constant?
  if (IsConstant(first))
  {
    uint64_t value = RipBits(type.BitWidth(), constantContainer.at(first));
    return value != 0;
  }

  //checks in constraint container
  //------------------------------------------

  //loop through vector of constraintIds corresponding to a first ValueId
  for (auto constrId : GetConstraintIdVector(first))
  {
    auto constr = constrContainer.find(constrId);
    if (constr == constrContainer.end())
      throw std::runtime_error("Referenced constraint does not exist!");

    //check direct relation
    ValueId second = constr->second.GetOther(first);

    //do we have constraint with a constant?
    if (IsConstant(second))
    {
      uint64_t value = RipBits(type.BitWidth(), constantContainer.at(second));

      switch (constr->second.relation)
      {
      case CmpFlags::Eq:
        return value != 0;
      case CmpFlags::Neq:
        if (value == 0)
          return false;
        break;

      //TODO::check for intervals
      //NOTE::need to check ValueId swapping

      default:
        break;
      }

    }

    //TODO::Check in other layers

  }

  return boost::indeterminate;
}

boost::tribool ValueContainer::IsFalse(ValueId first, Type type) const
{
  return !IsTrue(first, type);
}

boost::tribool ValueContainer::IsBinaryEq(ValueId first, ValueId second) const
{
  if (first == second)
    return true;
  
  //if both constants but different, return false
  if (IsConstant(first) && IsConstant(second))
    return false;


  //check for equality in constraint container
  //------------------------------------------
  if (first > second)
    std::swap(first, second);


  //loop through vector of constraintIds corresponding to the first ValueId
  for (auto constrId : GetConstraintIdVector(first))
  {
    auto constr = constrContainer.find(constrId);
    if (constr == constrContainer.end())
      throw std::runtime_error("Referenced constraint does not exist!");

    if (constr->second.Contains(second))
    {
      //constraint between both ValueIds
      switch (constr->second.relation)
      {
      case CmpFlags::Eq:
        return true;
      case CmpFlags::Neq:
        return false;
      default:
        break;
      }
    }

    //TODO::check intervals
  }


  return boost::indeterminate;
}

boost::tribool ValueContainer::IsUnknown(ValueId first) const
{
  return constantContainer.find(first) == constantContainer.end();
  //TODO::unknown unknown?
}

boost::tribool ValueContainer::IsZero(ValueId first) const
{
  auto lhs = constantContainer.find(first);

  //is a constant?
  if (lhs != constantContainer.end())
  {
    uint64_t value = lhs->second;
    return value == 0;
  }

  //checks in constraint container
  //------------------------------------------
  //loop through vector of constraintIds
  for (const auto constrId : GetConstraintIdVector(first))
  {
    auto constr = constrContainer.find(constrId);
    if (constr == constrContainer.end())
      throw std::runtime_error("Referenced constraint does not exist!");

    //check direct relation
    ValueId second = constr->second.GetOther(first);
    
    if(IsConstant(second))
    {
      //both first and second are constants
      uint64_t value = constantContainer.at(second);

      switch (constr->second.relation)
      {
      case CmpFlags::Eq:
        return value == 0;
      case CmpFlags::Neq:
        if (value == 0)
          return true;
        break;

        //TODO::check for intervals

      default:
        break;
      }

    }

    //TODO::Check in other layers, maybe recursion?
  }

  return boost::indeterminate;
}

void ValueContainer::Assume(ValueId first, ValueId second, Type type, CmpFlags flags)
{
  InsertConstraint(BinaryConstraint(first, second, flags));
}

void ValueContainer::AssumeTrue(ValueId first)
{
  ValueId zero = CreateConstIntVal(0);
  InsertConstraint(BinaryConstraint(first, zero, CmpFlags::Neq)); ///TODO::not equal 0? or equal to 1?? logical comparation always 1?
}

void ValueContainer::AssumeFalse(ValueId first)
{
  ValueId zero = CreateConstIntVal(0);
  InsertConstraint(BinaryConstraint(first, zero, CmpFlags::Eq));
}


ValueId ValueContainer::Add(ValueId first, ValueId second, Type type, ArithFlags flags)
{
  auto lhs = constantContainer.find(first);
  auto rhs = constantContainer.find(second);

  //not constants, return unknown
  if (lhs == constantContainer.end() || rhs == constantContainer.end())
    return CreateVal(type);

  //both constants
  if (CHECK_FLAG(flags, ArithFlags::Unsigned))
  {
    //prepare values
    uint64_t val1 = RipBits(type.BitWidth() ,lhs->second);
    uint64_t val2 = RipBits(type.BitWidth(), rhs->second);
    uint64_t max = RipBits(type.BitWidth(), ULLONG_MAX);
    
    if (CHECK_FLAG(flags, ArithFlags::NoUnsignedWrap))
    {
      if (max - val1 < val2)
        return CreateVal(type);
    }
    
    uint64_t result = RipBits(type.BitWidth(), val1 + val2);
    return CreateConstIntVal(result, type);


  }
  else
  {
    //signed
    uint64_t val1 = SignExtend64(type.BitWidth(), lhs->second);
    uint64_t val2 = SignExtend64(type.BitWidth(), rhs->second);

    uint64_t max = RipBits(type.BitWidth() - 1, ULLONG_MAX);
    uint64_t min = (1ULL << (type.BitWidth() - 1));

    if (CHECK_FLAG(flags, ArithFlags::NoSignedWrap))
    {
      //check overflow
      if (val1 > 0 && val2 > 0 && max - val1 < val2)
        return CreateVal(type);
      //check underflow
      else if(val1 < 0 && val2 < 0 && min - val1 > val2)
        return CreateVal(type);
    }

    uint64_t result = SignExtend64(type.BitWidth(), val1 + val2);
    return CreateConstIntVal(result, type);
    
  }

}

ValueId ValueContainer::Sub(ValueId first, ValueId second, Type type, ArithFlags flags)
{
  auto lhs = constantContainer.find(first);
  auto rhs = constantContainer.find(second);

  //not constants, return unknown
  if (lhs == constantContainer.end() || rhs == constantContainer.end())
    return CreateVal(type);

  //both constants
  if (CHECK_FLAG(flags, ArithFlags::Unsigned))
  {
    //prepare values
    uint64_t val1 = RipBits(type.BitWidth(), lhs->second);
    uint64_t val2 = RipBits(type.BitWidth(), rhs->second);

    if (CHECK_FLAG(flags, ArithFlags::NoUnsignedWrap))
    {
      if (val1 < val2)
        return CreateVal(type);
    }

    uint64_t result = RipBits(type.BitWidth(), val1 - val2);
    return CreateConstIntVal(result, type);


  }
  else
  {
    //signed
    uint64_t val1 = SignExtend64(type.BitWidth(), lhs->second);
    uint64_t val2 = SignExtend64(type.BitWidth(), rhs->second);

    uint64_t max = RipBits(type.BitWidth() - 1, ULLONG_MAX);
    uint64_t min = (1ULL << (type.BitWidth() - 1));

    if (CHECK_FLAG(flags, ArithFlags::NoSignedWrap))
    {
      //check overflow
      if (val1 > 0 && val2 < 0 && max + val2 < val1)
        return CreateVal(type);
      //check underflow
      else if (val1 < 0 && val2 > 0 && min + val2 > val1)
        return CreateVal(type);
    }

    uint64_t result = SignExtend64(type.BitWidth(), val1 - val2);
    return CreateConstIntVal(result, type);

  }
}

ValueId ValueContainer::Mul(ValueId first, ValueId second, Type type, ArithFlags flags)
{
  auto lhs = constantContainer.find(first);
  auto rhs = constantContainer.find(second);

  //not constants, return unknown
  if (lhs == constantContainer.end() || rhs == constantContainer.end())
    return CreateVal(type);

  //both constants
  if (CHECK_FLAG(flags, ArithFlags::Unsigned))
  {
    //prepare values
    uint64_t val1 = RipBits(type.BitWidth(), lhs->second);
    uint64_t val2 = RipBits(type.BitWidth(), rhs->second);
    

    if (CHECK_FLAG(flags, ArithFlags::NoUnsignedWrap))
    {
      //on how many bits we need to represent the result?
      uint64_t bitsNeeded = BSR(val1) + BSR(val2);

      //overflow
      if (bitsNeeded > type.BitWidth())
        return CreateVal(type);
    }

    uint64_t result = RipBits(type.BitWidth(), val1 * val2);
    return CreateConstIntVal(result, type);


  }
  else
  {
    //signed
    int64_t val1 = (int64_t)SignExtend64(type.BitWidth(), lhs->second);
    int64_t val2 = (int64_t)SignExtend64(type.BitWidth(), rhs->second);

    if (CHECK_FLAG(flags, ArithFlags::NoSignedWrap))
    {
      uint64_t bitsNeeded = BSR(val1 < 0 ? (uint64_t)(-val1) : (uint64_t)val1) + BSR(val2 < 0 ? (uint64_t)(-val2) : (uint64_t)val2);

      //check overflow
      if (bitsNeeded > type.BitWidth())
        return CreateVal(type);
    }

    uint64_t result = SignExtend64(type.BitWidth(), val1 * val2);
    return CreateConstIntVal(result, type);

  }
}

ValueId ValueContainer::Div(ValueId first, ValueId second, Type type, ArithFlags flags)
{
  auto lhs = constantContainer.find(first);
  auto rhs = constantContainer.find(second);

  //not constants, return unknown
  if (lhs == constantContainer.end() || rhs == constantContainer.end())
    return CreateVal(type);
    

  if (CHECK_FLAG(flags, ArithFlags::Unsigned))
  {
    //prepare values
    uint64_t val1 = RipBits(type.BitWidth(), lhs->second);
    uint64_t val2 = RipBits(type.BitWidth(), rhs->second);
    
    //divide by zero - what to do?
    if(val2 == 0)
      return CreateVal(type);

    uint64_t result = RipBits(type.BitWidth(), val1 / val2);
    return CreateConstIntVal(result, type);

  }
  else
  {
    //signed
    uint64_t val1 = SignExtend64(type.BitWidth(), lhs->second);
    uint64_t val2 = SignExtend64(type.BitWidth(), rhs->second);

    //divide by zero - what to do?
    if (val2 == 0)
      return CreateVal(type);

    uint64_t result = SignExtend64(type.BitWidth(), val1 / val2);
    return CreateConstIntVal(result, type);

  }
}

ValueId ValueContainer::Rem(ValueId first, ValueId second, Type type, ArithFlags flags)
{
  auto lhs = constantContainer.find(first);
  auto rhs = constantContainer.find(second);

  //not constants, return unknown
  if (lhs == constantContainer.end() || rhs == constantContainer.end())
    return CreateVal(type);


  if (CHECK_FLAG(flags, ArithFlags::Unsigned))
  {
    //prepare values
    uint64_t val1 = RipBits(type.BitWidth(), lhs->second);
    uint64_t val2 = RipBits(type.BitWidth(), rhs->second);

    //divide by zero - what to do?
    if (val2 == 0)
      return CreateVal(type);

    uint64_t result = RipBits(type.BitWidth(), val1 % val2);
    return CreateConstIntVal(result, type);

  }
  else
  {
    //signed
    uint64_t val1 = SignExtend64(type.BitWidth(), lhs->second);
    uint64_t val2 = SignExtend64(type.BitWidth(), rhs->second);

    //divide by zero - what to do?
    if (val2 == 0)
      return CreateVal(type);

    uint64_t result = SignExtend64(type.BitWidth(), val1 % val2);
    return CreateConstIntVal(result, type);

  }
}

ValueId ValueContainer::LSh(ValueId first, ValueId second, Type type, ArithFlags flags)
{
  auto lhs = constantContainer.find(first);
  auto rhs = constantContainer.find(second);

  //if both are not constants, return unkown
  if (lhs == constantContainer.end() || rhs == constantContainer.end())
    return CreateVal(type);

  size_t numOfBits = type.BitWidth();
  uint64_t value = RipBits(numOfBits, lhs->second);
  uint64_t shiftval = RipBits(numOfBits, rhs->second);

  value <<= shiftval;

  return CreateConstIntVal(value, type);
}

ValueId ValueContainer::RSh(ValueId first, ValueId second, Type type, ArithFlags flags)
{
  auto lhs = constantContainer.find(first);
  auto rhs = constantContainer.find(second);

  //if both are not constants, return unkown
  if (lhs == constantContainer.end() || rhs == constantContainer.end())
    return CreateVal(type);

  size_t numOfBits = type.BitWidth();
  uint64_t value = RipBits(numOfBits, lhs->second);
  uint64_t shiftval = RipBits(numOfBits, rhs->second);
  bool isNegative = (bool)(value & (1ULL << (numOfBits - 1)));

  value >>= shiftval;

  //if sign, perform sign extention
  if (CHECK_FLAG(flags, ArithFlags::Signed) && isNegative)
  {
    int64_t origBits = numOfBits - shiftval;
    if (origBits < 0)
      value = (uint64_t)(-1LL);
    else
      value |= ~((1ULL << origBits) - 1);
  }

  return CreateConstIntVal(value, type);
}

ValueId ValueContainer::BitAnd(ValueId first, ValueId second, Type type)
{
  auto lhs = constantContainer.find(first);
  auto rhs = constantContainer.find(second);

  //if one of them is not a constant, return unkown
  if (lhs == constantContainer.end() || rhs == constantContainer.end())
    return CreateVal(type);

  uint64_t value = lhs->second & rhs->second;

  return CreateConstIntVal(value, type);
}

ValueId ValueContainer::BitOr(ValueId first, ValueId second, Type type)
{
  auto lhs = constantContainer.find(first);
  auto rhs = constantContainer.find(second);

  //if one of them is not a constant, return unkown
  if (lhs == constantContainer.end() || rhs == constantContainer.end())
    return CreateVal(type);

  uint64_t value = lhs->second | rhs->second;

  return CreateConstIntVal(value, type);
}

ValueId ValueContainer::BitXor(ValueId first, ValueId second, Type type)
{
  auto lhs = constantContainer.find(first);
  auto rhs = constantContainer.find(second);

  //if one of them is not a constant, return unkown
  if (lhs == constantContainer.end() || rhs == constantContainer.end())
    return CreateVal(type);

  uint64_t value = lhs->second ^ rhs->second;
  
  return CreateConstIntVal(value, type);
}

ValueId ValueContainer::BitNot(ValueId first, Type type)
{
  auto lhs = constantContainer.find(first);

  //if not a constant, return unkown
  if (lhs == constantContainer.end())
    return CreateVal(type);

  uint64_t value = ~(lhs->second);

  return CreateConstIntVal(value, type);
}

ValueId ValueContainer::LogAnd(ValueId first, ValueId second, Type type)
{
  boost::tribool result = IsTrue(first, type) && IsTrue(second, type);
  if (result == true)
    CreateConstIntVal(1, type);
  else if (result == false)
    return CreateConstIntVal(0, type);
  else
    return CreateVal(type);
}

ValueId ValueContainer::LogOr(ValueId first, ValueId second, Type type)
{
  boost::tribool result = IsTrue(first, type) || IsTrue(second, type);
  if (result == true)
    CreateConstIntVal(1, type);
  else if (result == false)
    return CreateConstIntVal(0, type);
  else
    return CreateVal(type);
}

ValueId ValueContainer::LogNot(ValueId first, Type type)
{
  boost::tribool result = !IsTrue(first, type);
  if (result == true)
    CreateConstIntVal(1, type);
  else if (result == false)
    return CreateConstIntVal(0, type);
  else
    return CreateVal(type);
}

ValueId ValueContainer::ExtendInt(ValueId first, Type sourceType, Type targetType, ArithFlags flags)
{
  auto val = constantContainer.find(first);

  //if unknown return new unkown
  if (val == constantContainer.end())
    return CreateVal(targetType);

  uint64_t value = val->second;	

  //if signed flag perform sign extention
  if (CHECK_FLAG(flags, ArithFlags::Signed))
    value = SignExtend64(sourceType.BitWidth(), value);
  else
    value = RipBits(sourceType.BitWidth(), value);

  //find or create ValueId
  return CreateConstIntVal(value, targetType);
}

ValueId ValueContainer::TruncInt(ValueId first, Type sourceType, Type targetType)
{
  auto val = constantContainer.find(first);

  //if not already in const container return new unknown
  if (val == constantContainer.end())
    return CreateVal(targetType);
  
  else
    return first;
}

ValueId ValueContainer::CreateConstIntVal(uint64_t value, Type type)
{
  //if exists return existing
  for (auto &item : constantContainer)
  {
    if (item.second == value)
      return item.first;
  }

  //else create new
  ValueId ret = ValueId::GetNextId();
  constantContainer[ret] = value;

  return ret;
}

ValueId ValueContainer::CreateConstIntVal(uint64_t value)
{
  //if exists return existing
  for (auto &item : constantContainer)
  {
    if (item.second == value)
      return item.first;
  }

  //else create new
  ValueId ret = ValueId::GetNextId();
  constantContainer[ret] = value;

  return ret;
}

ValueId ValueContainer::CreateConstFloatVal(float value, Type type)
{
  throw NotSupportedException();
}

ValueId ValueContainer::CreateConstFloatVal(double value, Type type)
{
  throw NotSupportedException();
}
