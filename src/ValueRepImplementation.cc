#include "ValueRepImplementation.hh"

#define CHECK_FLAG(var, flag) ((var & flag) == flag)

BinaryConstraint::BinaryConstraint(ValueId _first, ValueId _second, CmpFlags _relation)
{
	if (first > second)
	{
		std::swap(first, second);
		switch(_relation)
		{
		case CmpFlags::Gt:
			_relation = CmpFlags::Lt;
			break;
		case CmpFlags::GtEq:
			_relation = CmpFlags::LtEq;
			break;
		case CmpFlags::Lt:
			_relation = CmpFlags::Gt;
			break;
		case CmpFlags::LtEq:
			_relation = CmpFlags::GtEq;
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
			auto rhsConstraintIds = constrIdContainer.find(constr.second);
			for (auto oldConstraintId : rhsConstraintIds->second)
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
			auto lhsConstraintIds = constrIdContainer.find(constr.first);
			for (auto oldConstraintId : lhsConstraintIds->second)
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
	else
	{
		//not a relation with a constant -> invalidate only constraints with each other
		auto lhs = constrIdContainer.find(constr.first);
		if (lhs != constrIdContainer.end())
		{
			for (auto oldConstraintId : lhs->second)
			{
				if (constr.second == constrContainer[oldConstraintId].GetOther(constr.first))
				{
					//delete previous mutual constraints
					DeleteConstraint(oldConstraintId);
				}

			}
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


boost::tribool ValueContainer::IsCmp(ValueId first, ValueId second, Type type, CmpFlags flags) const
{
	//NOTE::first and second must be sorted

	if (IsConstant(first))
	{
		if (IsConstant(second))
		{
			//both are constants

			if(CHECK_FLAG(flags, CmpFlags::Signed))
			{
				int64_t val1 = (int64_t)SignExtend64(type.BitWidth(), eqContainer.at(first));
				int64_t val2 = (int64_t)SignExtend64(type.BitWidth(), eqContainer.at(second));

				switch (flags)
				{
				case CmpFlags::Eq:
					return val1 == val2;
				case CmpFlags::Neq:
					return val1 != val2;
				case CmpFlags::Gt:
					return  val1 > val2;
				case CmpFlags::GtEq:
					return  val1 >= val2;
				case CmpFlags::Lt:
					return  val1 < val2;
				case CmpFlags::LtEq:
					return  val1 <= val2;
				default:
					break;
				}
			}
			else
			{
				uint64_t val1 = ZeroExtend64(type.BitWidth(), eqContainer.at(first));
				uint64_t val2 = ZeroExtend64(type.BitWidth(), eqContainer.at(second));

				switch (flags)
				{
				case CmpFlags::Eq:
					return val1 == val2;
				case CmpFlags::Neq:
					return val1 != val2;
				case CmpFlags::Gt:
					return  val1 > val2;
				case CmpFlags::GtEq:
					return  val1 >= val2;
				case CmpFlags::Lt:
					return  val1 < val2;
				case CmpFlags::LtEq:
					return  val1 <= val2;
				default:
					break;
				}
			}
		}
		else
		{
			//only first is a constant
			if (CHECK_FLAG(flags, CmpFlags::Signed))
			{
				int64_t val1 = SignExtend64(type.BitWidth(), eqContainer.at(first));

				auto rhsConstraintIds = constrIdContainer.find(second);
				if (rhsConstraintIds != constrIdContainer.end())
				{
					for (auto constraintId : rhsConstraintIds->second)
					{
						auto constraint = constrContainer.at(constraintId);
						ValueId other = constraint.GetOther(second);
						if (IsConstant(other))
						{
							int64_t val2 = SignExtend64(type.BitWidth(), eqContainer.at(other));
							switch (flags)
							{
							case CmpFlags::Eq:
								if (constraint.relation == CmpFlags::Eq)
									return val1 == val2;
								if (constraint.relation == CmpFlags::Neq && val1 == val2)
									return false;
								//add intervals -- needs even more layers first<>second in constraint...
								break;
							case CmpFlags::Neq:
								if (constraint.relation == CmpFlags::Eq)
									return val1 != val2;
								if (constraint.relation == CmpFlags::Neq && val1 == val2)
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
			}
			else
			{
				//unsigned
				uint64_t val1 = ZeroExtend64(type.BitWidth(), eqContainer.at(first));

				auto rhsConstraintIds = constrIdContainer.find(second);
				if (rhsConstraintIds != constrIdContainer.end())
				{
					for (auto constraintId : rhsConstraintIds->second)
					{
						auto constraint = constrContainer.at(constraintId);
						ValueId other = constraint.GetOther(second);
						if (IsConstant(other))
						{
							uint64_t val2 = ZeroExtend64(type.BitWidth(), eqContainer.at(other));
							switch (flags)
							{
							case CmpFlags::Eq:
								if (constraint.relation == CmpFlags::Eq)
									return val1 == val2;
								if (constraint.relation == CmpFlags::Neq && val1 == val2)
									return false;
								//add intervals -- needs even more layers first<>second in constraint...
								break;
							case CmpFlags::Neq:
								if (constraint.relation == CmpFlags::Eq)
									return val1 != val2;
								if (constraint.relation == CmpFlags::Neq && val1 == val2)
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
			}

		}
	}
	else if (IsConstant(second))
	{
		//only a second is a constant
		if (CHECK_FLAG(flags, CmpFlags::Signed))
		{
			int64_t val1 = SignExtend64(type.BitWidth(), eqContainer.at(second));

			auto rhsConstraintIds = constrIdContainer.find(first);
			if (rhsConstraintIds != constrIdContainer.end())
			{
				for (auto constraintId : rhsConstraintIds->second)
				{
					auto constraint = constrContainer.at(constraintId);
					ValueId other = constraint.GetOther(first);
					if (IsConstant(other))
					{
						int64_t val2 = SignExtend64(type.BitWidth(), eqContainer.at(other));
						switch (flags)
						{
						case CmpFlags::Eq:
							if (constraint.relation == CmpFlags::Eq)
								return val1 == val2;
							if (constraint.relation == CmpFlags::Neq && val1 == val2)
								return false;
							//add intervals -- needs even more layers first<>second in constraint...
							break;
						case CmpFlags::Neq:
							if (constraint.relation == CmpFlags::Eq)
								return val1 != val2;
							if (constraint.relation == CmpFlags::Neq && val1 == val2)
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
		}
		else
		{
			//unsigned
			uint64_t val1 = ZeroExtend64(type.BitWidth(), eqContainer.at(second));

			auto rhsConstraintIds = constrIdContainer.find(first);
			if (rhsConstraintIds != constrIdContainer.end())
			{
				for (auto constraintId : rhsConstraintIds->second)
				{
					auto constraint = constrContainer.at(constraintId);
					ValueId other = constraint.GetOther(first);
					if (IsConstant(other))
					{
						uint64_t val2 = ZeroExtend64(type.BitWidth(), eqContainer.at(other));
						switch (flags)
						{
						case CmpFlags::Eq:
							if (constraint.relation == CmpFlags::Eq)
								return val1 == val2;
							if (constraint.relation == CmpFlags::Neq && val1 == val2)
								return false;
							//add intervals -- needs even more layers first<>second in constraint...
							break;
						case CmpFlags::Neq:
							if (constraint.relation == CmpFlags::Eq)
								return val1 != val2;
							if (constraint.relation == CmpFlags::Neq && val1 == val2)
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
		}
	}
	else
	{
		//neither are a constant
		auto lhsConstrIds = constrIdContainer.find(first);
		if(lhsConstrIds != constrIdContainer.end())
		{ 
			for(auto constrId : lhsConstrIds->second)
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

	}

	return boost::indeterminate;
}

boost::tribool ValueContainer::IsEq(ValueId first, ValueId second, Type type) const
{
	if (first == second)
		return true;

	auto lhs = eqContainer.find(first);
	auto rhs = eqContainer.find(second);

	//if both constants but different
	if (lhs != eqContainer.end() && rhs != eqContainer.end())
		return ZeroExtend64(type.BitWidth(), lhs->second) == ZeroExtend64(type.BitWidth(), rhs->second);

	//check for equality in constraint container
	//------------------------------------------
	if (second > first)
		std::swap(first, second);

	auto lhsConstrId = constrIdContainer.find(first);

	//does first have constraints
	if (lhsConstrId == constrIdContainer.end())
		return boost::indeterminate;

	//loop through vector of constraintIds
	for (auto constrId : lhsConstrId->second)
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
			case CmpFlags::Gt:
			case CmpFlags::Lt:
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
	if (first == second)
		return false;

	auto lhs = eqContainer.find(first);
	auto rhs = eqContainer.find(second);

	//if are different constants, return true
	if (lhs != eqContainer.end() && rhs != eqContainer.end())
		return ZeroExtend64(type.BitWidth(), lhs->second) != ZeroExtend64(type.BitWidth(), rhs->second);

	//check for equality in constraint container
	//------------------------------------------
	if (second > first)
		std::swap(first, second);

	auto lhsConstrId = constrIdContainer.find(first);

	//does first have constraints
	if (lhsConstrId == constrIdContainer.end())
		return boost::indeterminate;

	//loop through vector of constraintIds
	for (auto constrId : lhsConstrId->second)
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
				return false;
			case CmpFlags::Neq:
			case CmpFlags::Gt:
			case CmpFlags::Lt:
				return true;
			default:
				break;
			}
		}

		//TODO::check intervals
	}


	return boost::indeterminate;
}

boost::tribool ValueContainer::IsTrue(ValueId first, Type type) const
{
	auto lhs = eqContainer.find(first);

	//is a constant?
	if (lhs != eqContainer.end())
	{
		uint64_t value = ZeroExtend64(type.BitWidth(), lhs->second);
		return value != 0;
	}

	//checks in constraint container
	//------------------------------------------

	auto lhsConstrId = constrIdContainer.find(first);

	//does first have constraints
	if (lhsConstrId == constrIdContainer.end())
		return boost::indeterminate;

	//loop through vector of constraintIds
	for (auto constrId : lhsConstrId->second)
	{
		auto constr = constrContainer.find(constrId);
		if (constr == constrContainer.end())
			throw std::runtime_error("Referenced constraint does not exist!");

		//check direct relation
		ValueId second = constr->second.GetOther(first);
		auto constant = eqContainer.find(second);

		//do we have constraint with a constant?
		if (constant != eqContainer.end())
		{
			uint64_t value = ZeroExtend64(type.BitWidth(), constant->second);

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
	auto lhs = eqContainer.find(first);

	//is a constant?
	if (lhs != eqContainer.end())
	{
		uint64_t value = ZeroExtend64(type.BitWidth(), lhs->second);
		return value == 0;
	}

	//checks in constraint container
	//------------------------------------------

	auto lhsConstrId = constrIdContainer.find(first);

	//does first have constraints
	if (lhsConstrId == constrIdContainer.end())
		return boost::indeterminate;

	//loop through vector of constraintIds
	for (auto constrId : lhsConstrId->second)
	{
		auto constr = constrContainer.find(constrId);
		if (constr == constrContainer.end())
			throw std::runtime_error("Referenced constraint does not exist!");

		//check direct relation
		ValueId second = constr->second.GetOther(first);
		auto constant = eqContainer.find(second);

		//do we have constraint with a constant?
		if (constant != eqContainer.end())
		{
			uint64_t value = ZeroExtend64(type.BitWidth(), constant->second);

			switch (constr->second.relation)
			{
			case CmpFlags::Eq:
				return value == 0;
			case CmpFlags::Neq:
				if (value == 0)
					return true;
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

boost::tribool ValueContainer::IsBinaryEq(ValueId first, ValueId second) const
{
	if (first == second)
		return true;

	auto lhs = eqContainer.find(first);
	auto rhs = eqContainer.find(second);

	//if both constants but different, return false
	if (lhs != eqContainer.end() && rhs != eqContainer.end())
		return false;

	//check for equality in constraint container
	//------------------------------------------
	if (second > first)
		std::swap(first, second);

	auto lhsConstrId = constrIdContainer.find(first);

	//does first have constraints
	if (lhsConstrId == constrIdContainer.end())
		return boost::indeterminate;

	//loop through vector of constraintIds
	for (auto constrId : lhsConstrId->second)
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
	return eqContainer.find(first) == eqContainer.end();
	//unknown unknown?
}

boost::tribool ValueContainer::IsZero(ValueId first) const
{
	auto lhs = eqContainer.find(first);

	//is a constant?
	if (lhs != eqContainer.end())
	{
		uint64_t value = lhs->second;
		return value == 0;
	}

	//checks in constraint container
	//------------------------------------------

	auto lhsConstrId = constrIdContainer.find(first);

	//does first have constraints
	if (lhsConstrId == constrIdContainer.end())
		return boost::indeterminate;

	//loop through vector of constraintIds
	for (auto constrId : lhsConstrId->second)
	{
		auto constr = constrContainer.find(constrId);
		if (constr == constrContainer.end())
			throw std::runtime_error("Referenced constraint does not exist!");

		//check direct relation
		ValueId second = constr->second.GetOther(first);
		auto constant = eqContainer.find(second);

		//do we have constraint with a constant?
		if (constant != eqContainer.end())
		{
			uint64_t value = constant->second;

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

		//TODO::Check in other layers

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
	InsertConstraint(BinaryConstraint(first, zero, CmpFlags::Neq));
}

void ValueContainer::AssumeFalse(ValueId first)
{
	ValueId zero = CreateConstIntVal(0);
	InsertConstraint(BinaryConstraint(first, zero, CmpFlags::Eq));
}


ValueId ValueContainer::Add(ValueId first, ValueId second, Type type, ArithFlags flags)
{
	auto lhs = eqContainer.find(first);
	auto rhs = eqContainer.find(second);

	//not constants, return unknown
	if (lhs == eqContainer.end() || rhs == eqContainer.end())
		return CreateVal(type);

	//both constants
	if (CHECK_FLAG(flags, ArithFlags::Unsigned))
	{
		//prepare values
		uint64_t val1 = ZeroExtend64(type.BitWidth() ,lhs->second);
		uint64_t val2 = ZeroExtend64(type.BitWidth(), rhs->second);
		uint64_t max = ZeroExtend64(type.BitWidth(), ULLONG_MAX);
		
		if (CHECK_FLAG(flags, ArithFlags::NoUnsignedWrap))
		{
			if (max - val1 < val2)
				return CreateVal(type);
		}
		
		uint64_t result = ZeroExtend64(type.BitWidth(), val1 + val2);
		return CreateConstIntVal(result, type);


	}
	else
	{
		//signed
		uint64_t val1 = SignExtend64(type.BitWidth(), lhs->second);
		uint64_t val2 = SignExtend64(type.BitWidth(), rhs->second);

		uint64_t max = ZeroExtend64(type.BitWidth() - 1, ULLONG_MAX);
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
	auto lhs = eqContainer.find(first);
	auto rhs = eqContainer.find(second);

	//not constants, return unknown
	if (lhs == eqContainer.end() || rhs == eqContainer.end())
		return CreateVal(type);

	//both constants
	if (CHECK_FLAG(flags, ArithFlags::Unsigned))
	{
		//prepare values
		uint64_t val1 = ZeroExtend64(type.BitWidth(), lhs->second);
		uint64_t val2 = ZeroExtend64(type.BitWidth(), rhs->second);

		if (CHECK_FLAG(flags, ArithFlags::NoUnsignedWrap))
		{
			if (val1 < val2)
				return CreateVal(type);
		}

		uint64_t result = ZeroExtend64(type.BitWidth(), val1 - val2);
		return CreateConstIntVal(result, type);


	}
	else
	{
		//signed
		uint64_t val1 = SignExtend64(type.BitWidth(), lhs->second);
		uint64_t val2 = SignExtend64(type.BitWidth(), rhs->second);

		uint64_t max = ZeroExtend64(type.BitWidth() - 1, ULLONG_MAX);
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
	auto lhs = eqContainer.find(first);
	auto rhs = eqContainer.find(second);

	//not constants, return unknown
	if (lhs == eqContainer.end() || rhs == eqContainer.end())
		return CreateVal(type);

	//both constants
	if (CHECK_FLAG(flags, ArithFlags::Unsigned))
	{
		//prepare values
		uint64_t val1 = ZeroExtend64(type.BitWidth(), lhs->second);
		uint64_t val2 = ZeroExtend64(type.BitWidth(), rhs->second);
		

		if (CHECK_FLAG(flags, ArithFlags::NoUnsignedWrap))
		{
			//on how many bits we need to represent the result?
			uint64_t bitsNeeded = BSD(val1) + BSD(val2);

			//overflow
			if (bitsNeeded > type.BitWidth())
				return CreateVal(type);
		}

		uint64_t result = ZeroExtend64(type.BitWidth(), val1 * val2);
		return CreateConstIntVal(result, type);


	}
	else
	{
		//signed
		uint64_t val1 = SignExtend64(type.BitWidth(), lhs->second);
		uint64_t val2 = SignExtend64(type.BitWidth(), rhs->second);

		if (CHECK_FLAG(flags, ArithFlags::NoSignedWrap))
		{
			uint64_t bitsNeeded = BSD(val1 < 0 ? -val1 : val1) + BSD(val2 < 0 ? -val2 : val2);

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
	auto lhs = eqContainer.find(first);
	auto rhs = eqContainer.find(second);

	//not constants, return unknown
	if (lhs == eqContainer.end() || rhs == eqContainer.end())
		return CreateVal(type);
		

	if (CHECK_FLAG(flags, ArithFlags::Unsigned))
	{
		//prepare values
		uint64_t val1 = ZeroExtend64(type.BitWidth(), lhs->second);
		uint64_t val2 = ZeroExtend64(type.BitWidth(), rhs->second);
		
		//divide by zero - what to do?
		if(val2 == 0)
			return CreateVal(type);

		uint64_t result = ZeroExtend64(type.BitWidth(), val1 / val2);
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
	auto lhs = eqContainer.find(first);
	auto rhs = eqContainer.find(second);

	//not constants, return unknown
	if (lhs == eqContainer.end() || rhs == eqContainer.end())
		return CreateVal(type);


	if (CHECK_FLAG(flags, ArithFlags::Unsigned))
	{
		//prepare values
		uint64_t val1 = ZeroExtend64(type.BitWidth(), lhs->second);
		uint64_t val2 = ZeroExtend64(type.BitWidth(), rhs->second);

		//divide by zero - what to do?
		if (val2 == 0)
			return CreateVal(type);

		uint64_t result = ZeroExtend64(type.BitWidth(), val1 % val2);
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
	auto lhs = eqContainer.find(first);
	auto rhs = eqContainer.find(second);

	//if both are not constants, return unkown
	if (lhs == eqContainer.end() || rhs == eqContainer.end())
		return CreateVal(type);

	size_t numOfBits = type.BitWidth();
	uint64_t value = ZeroExtend64(numOfBits, lhs->second);
	uint64_t shiftval = ZeroExtend64(numOfBits, rhs->second);

	value <<= shiftval;

	return CreateConstIntVal(value, type);
}

ValueId ValueContainer::RSh(ValueId first, ValueId second, Type type, ArithFlags flags)
{
	auto lhs = eqContainer.find(first);
	auto rhs = eqContainer.find(second);

	//if both are not constants, return unkown
	if (lhs == eqContainer.end() || rhs == eqContainer.end())
		return CreateVal(type);

	size_t numOfBits = type.BitWidth();
	uint64_t value = ZeroExtend64(numOfBits, lhs->second);
	uint64_t shiftval = ZeroExtend64(numOfBits, rhs->second);
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
	auto lhs = eqContainer.find(first);
	auto rhs = eqContainer.find(second);

	//if one of them is not a constant, return unkown
	if (lhs == eqContainer.end() || rhs == eqContainer.end())
		return CreateVal(type);

	uint64_t value = lhs->second & rhs->second;

	return CreateConstIntVal(value, type);
}

ValueId ValueContainer::BitOr(ValueId first, ValueId second, Type type)
{
	auto lhs = eqContainer.find(first);
	auto rhs = eqContainer.find(second);

	//if one of them is not a constant, return unkown
	if (lhs == eqContainer.end() || rhs == eqContainer.end())
		return CreateVal(type);

	uint64_t value = lhs->second | rhs->second;

	return CreateConstIntVal(value, type);
}

ValueId ValueContainer::BitXor(ValueId first, ValueId second, Type type)
{
	auto lhs = eqContainer.find(first);
	auto rhs = eqContainer.find(second);

	//if one of them is not a constant, return unkown
	if (lhs == eqContainer.end() || rhs == eqContainer.end())
		return CreateVal(type);

	uint64_t value = lhs->second ^ rhs->second;
	
	return CreateConstIntVal(value, type);
}

ValueId ValueContainer::BitNot(ValueId first, Type type)
{
	auto lhs = eqContainer.find(first);

	//if not a constant, return unkown
	if (lhs == eqContainer.end())
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
		CreateConstIntVal(0, type);
	else
		CreateVal(type);
}

ValueId ValueContainer::LogOr(ValueId first, ValueId second, Type type)
{
	boost::tribool result = IsTrue(first, type) || IsTrue(second, type);
	if (result == true)
		CreateConstIntVal(1, type);
	else if (result == false)
		CreateConstIntVal(0, type);
	else
		CreateVal(type);
}

ValueId ValueContainer::LogNot(ValueId first, Type type)
{
	boost::tribool result = !IsTrue(first, type);
	if (result == true)
		CreateConstIntVal(1, type);
	else if (result == false)
		CreateConstIntVal(0, type);
	else
		CreateVal(type);
}

ValueId ValueContainer::ExtendInt(ValueId first, Type sourceType, Type targetType, ArithFlags flags)
{
	auto val = eqContainer.find(first);

	//if unknown return new unkown
	if (val == eqContainer.end())
		return CreateVal(targetType);

	uint64_t value = val->second;	

	//if signed flag perform sign extention
	if (CHECK_FLAG(flags, ArithFlags::Signed))
		value = SignExtend64(sourceType.BitWidth(), value);
	else
		value = ZeroExtend64(sourceType.BitWidth(), value);

	//find or create ValueId
	return CreateConstIntVal(value, targetType);
}

ValueId ValueContainer::TruncInt(ValueId first, Type sourceType, Type targetType)
{
	auto val = eqContainer.find(first);

	//if not already in const container return new unknown
	if (val == eqContainer.end())
		return CreateVal(targetType);
	
	else
		return first;
}

ValueId ValueContainer::CreateConstIntVal(uint64_t value, Type type)
{
	//if exists return existing
	for (auto &item : eqContainer)
	{
		if (item.second == value)
			return item.first;
	}

	//else create new
	ValueId ret = ValueId::GetNextId();
	eqContainer[ret] = value;

	return ret;
}

ValueId ValueContainer::CreateConstIntVal(uint64_t value)
{
	//if exists return existing
	for (auto &item : eqContainer)
	{
		if (item.second == value)
			return item.first;
	}

	//else create new
	ValueId ret = ValueId::GetNextId();
	eqContainer[ret] = value;

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
