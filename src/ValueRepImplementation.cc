//
// Created by Charvin on 17. 10. 2016.
//

#include "ValueRepImplementation.hh"
#include <sstream>

static const char *ConstraintTypeRepresentation[] = {"<", ">", "<=", ">=", "==", "!="};

//INTERVAL CONSTRAINT CLASS IMPLEMENTATIONS
//-----------------------------------------

friend std::ostream& operator<<(std::ostream& stream, IntervalConstraint val)
{
	stream << ConstraintTypeRepresentation[static_cast<int>(val.type)] << " " << val.data.Integer;
	return stream;
}
//-----------------------------------------



//BINARY CONSTRAINT CLASS IMPLEMENTATIONS
//-----------------------------------------

BinaryConstraint::constraintIdGenerator = 0;
friend std::ostream& BinaryConstraint::operator<<(std::ostream& stream, BinaryConstraint val)
{
	stream << "  #" << val.id << ": #" << val.idVal1 << "  " << ConstraintTypeRepresentation[static_cast<int> (val.type)] << "  #" << val.idVal2 << std::endl;
	return stream;
}
//-----------------------------------------




//VALUE REPRESENTATION CLASS IMPLEMENTATIONS
//-----------------------------------------
ValueRep::valueIdGenerator = 0;


ValueRep::~ValueRep() {
	///TODO::remove constraints from tables

}


std::string ValueRep::ToString() const {
	std::stringstream buf;

	//interval constraints
	auto intervalIterator = intervalConstraintContainer.find(id);

	//check if ValueRep has interval constraints
	if(intervalIterator != intervalConstraintContainer.end())
	{
		//Append data from vector of constraints
		buf << "Interval Constraints for ValueRep " << id << ":\n___________________________________________" << std::endl;
		for(auto const& value : intervalIterator->second)
		{
			buf << " " << value << ", ";
		}
		buf << std::endl << std::endl;
	}

	//binary relation constraints
	auto binaryRelationIterator = binaryRelationIterator.find(id);

	//check if this ValueRep has binary constraints
	if(binaryRelationIterator != binaryRelationsContainer.end())
	{
		buf << "Binary Relation Constraints for ValueRep #" << id << " of type " << binaryRelationIterator->second.first << ":\n___________________________________________" << std::endl;

		for(auto const& value : binaryRelationIterator->second.second)
		{
			//check if constraint with id from binaryRelationContainer exists in binaryConstraintContainer, if not we are in deep sh*t
			auto constraintIterator = binaryConstraintContainer.find(value);
			if(constraintIterator == binaryConstraintContainer.end())
				throw std::logic_error("Binary relation container contains id of non existing constraint");

			buf << constraintIterator->second;
		}
	}
	return buf.str();
}

boost::tribool ValueRep::operator<(const IValueRep &other) const {
	return nullptr;
	///TODO::finish
}

boost::tribool ValueRep::operator>(const IValueRep &other) const {
	return nullptr;
	///TODO::finish
}

boost::tribool ValueRep::operator<=(const IValueRep &other) const {
	return nullptr;
	///TODO::finish
}

boost::tribool ValueRep::operator>=(const IValueRep &other) const {
	return nullptr;
	///TODO::finish
}

boost::tribool ValueRep::IsEQ(const IValueRep &other) const {
	return nullptr;
	///TODO::finish
}

boost::tribool ValueRep::IsNEQ(const IValueRep &other) const {
	return nullptr;
	///TODO::finish
}

boost::tribool ValueRep::operator<(const int64_t constValue) const {
	auto constraintVector = intervalConstraintContainer[id];

	for(auto& value : constraintVector)
	{
		switch(value.type)
		{
			case ConstraintType::eEQ:
				return value.data.Integer < constValue;

			case ConstraintType::eGT:
				if(value.data.Integer >= constValue - 1 )
					return false;
			case ConstraintType::eGTE:
				if(value.data.Integer  >= constValue)
					return false;
			case ConstraintType::eLT:
				if(value.data.Integer <= constValue)
					return true;
			case ConstraintType::eLTE:
				if(value.data.Integer < constValue)
					return true;
			default:
				break;
		}
	}

	return boost::indeterminate;
}

boost::tribool ValueRep::operator>(const int64_t constValue) const {
	auto constraintVector = intervalConstraintContainer[id];

	for(auto& value : constraintVector)
	{
		switch(value.type)
		{
			case ConstraintType::eEQ:
				return value.data.Integer > constValue;


			case ConstraintType::eGT:
				if(value.data.Integer >= constValue)
					return true;
			case ConstraintType::eGTE:
				if(value.data.Integer  > constValue)
					return true;
			case ConstraintType::eLT:
				if(value.data.Integer <= constValue + 1)
					return false;
			case ConstraintType::eLTE:
				if(value.data.Integer <= constValue)
					return false;
			default:
				break;
		}
	}

	return boost::indeterminate;
}

boost::tribool ValueRep::operator<=(const int64_t constValue) const {
	auto constraintVector = intervalConstraintContainer[id];

	for(auto& value : constraintVector)
	{
		switch(value.type)
		{
			case ConstraintType::eEQ:
				return value.data.Integer <= constValue;

			case ConstraintType::eGT:
				if(value.data.Integer >= constValue)
					return false;
			case ConstraintType::eGTE:
				if(value.data.Integer  > constValue)
					return false;
			case ConstraintType::eLT:
				if(value.data.Integer <= constValue + 1)
					return true;
			case ConstraintType::eLTE:
				if(value.data.Integer <= constValue)
					return true;
			default:
				break;
		}
	}

	return boost::indeterminate;
}

boost::tribool ValueRep::operator>=(const int64_t constValue) const {
	auto constraintVector = intervalConstraintContainer[id];

	for(auto& value : constraintVector)
	{
		switch(value.type)
		{
			case ConstraintType::eEQ:
				return value.data.Integer >= constValue;

			case ConstraintType::eGT:
				if(value.data.Integer >= constValue - 1)
					return true;
			case ConstraintType::eGTE:
				if(value.data.Integer  >= constValue)
					return true;
			case ConstraintType::eLT:
				if(value.data.Integer <= constValue)
					return false;
			case ConstraintType::eLTE:
				if(value.data.Integer < constValue)
					return false;
			default:
				break;
		}
	}

	return boost::indeterminate;
}

boost::tribool ValueRep::IsEQ(const int64_t constValue) const {
	auto constraintVector = intervalConstraintContainer[id];

	for(auto& value : constraintVector)
	{
		switch(value.type)
		{
			case ConstraintType::eEQ:
				return value.data.Integer == constValue;

			case ConstraintType::eNEQ:
				if(value.data.Integer == constValue)
					return false;
				break;

			case ConstraintType::eGT:
				if(value.data.Integer >= constValue)
					return false;
			case ConstraintType::eGTE:
				if(value.data.Integer  > constValue)
					return false;
			case ConstraintType::eLT:
				if(value.data.Integer <= constValue)
					return false;
			case ConstraintType::eLTE:
				if(value.data.Integer < constValue)
					return false;
			default:
				break;
		}
	}

	return boost::indeterminate;
}

boost::tribool ValueRep::IsNEQ(const int64_t constValue) const {
	auto constraintVector = intervalConstraintContainer[id];

	for(auto& value : constraintVector)
	{
		switch(value.type)
		{
			case ConstraintType::eEQ:
				return value.data.Integer == constValue;

			case ConstraintType::eNEQ:
				if(value.data.Integer == constValue)
					return false;
				break;

			case ConstraintType::eGT:
				if(value.data.Integer >= constValue)
					return false;
			case ConstraintType::eGTE:
				if(value.data.Integer  > constValue)
					return false;
			case ConstraintType::eLT:
				if(value.data.Integer <= constValue)
					return false;
			case ConstraintType::eLTE:
				if(value.data.Integer < constValue)
					return false;
			default:
				break;
		}

	}

	return boost::indeterminate;
}

boost::tribool ValueRep::IsUnknown() const {
	return boost::indeterminate;
}

void ValueRep::SetConstraint(ConstraintType type, IValueRep &other)
{
	//new constraint
	BinaryConstraint newConstraint(id, type, static_cast<ValueRep&>(other).id); ///TODO::is this typecast ok with c++???

	//register this constraint for IValueRep1
	binaryRelationsContainer[id].second.push_back(newConstraint.id);
	//register this constraint for IValueRep2
	binaryRelationsContainer[static_cast<ValueRep&>(other).id].second.push_back(newConstraint.id);

	//insert constraint
	binaryConstraintContainer[newConstraint.id] = newConstraint;

	//--> continue and fix all invalid constraints in the interval container...
}

void ValueRep::SetConstraint(ConstraintType type, int64_t constValue)
{

	auto constraintVector = intervalConstraintContainer[id];
	constraintVector.push_back(IntervalConstraint(type, constValue));


	//--> continue and fix all invalid constraints in the interval container...

	/*
	//get rid of non valid constraints
	for(auto constraintIterator = constraintVector.begin(); constraintIterator != constraintVector.end(); ++constraintIterator)
	{
		//decide what to do with all the different types of interval constraints
		if(constraintIterator->data.Integer == constValue) {
			switch(constraintIterator->type) {
				case ConstraintType::eEQ:
				case ConstraintType::eNEQ:
					constraintVector.erase(constraintIterator);
					break;
				case ConstraintType::eGTE:
					constraintIterator->type = ConstraintType::eGT;
					break;
				case ConstraintType::eLTE:
					constraintIterator->type = ConstraintType::eLT;
					break;
				default:
					break;
			}
		}
	}*/

	/*
	//get rid of non valid constraints
	for(auto constraintIterator = constraintVector.begin(); constraintIterator != constraintVector.end(); ++constraintIterator)
	{
		//decide what to do with all the different types of interval constraints
		if(constraintIterator->data.Integer == constValue) {
			switch(constraintIterator->type) {
				case ConstraintType::eEQ:
					constraintVector.erase(constraintIterator);
					break;
				case ConstraintType::eGTE:
					constraintIterator->type = ConstraintType::eGT;
					break;
				case ConstraintType::eLTE:
					constraintIterator->type = ConstraintType::eLT;
					break;
				default:
					break;
			}
		}
	}*/
}
//-----------------------------------------