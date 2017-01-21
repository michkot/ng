//
// Created by Charvin on 11. 11. 2016.
//
#pragma once

#include "Values.hh"
#include "Exceptions.hh"
#include <unordered_map>
#include <map>
#include <vector>


class BinaryConstraint {
public:
	ValueId first;
	ValueId second;
	CmpFlags relation;

	BinaryConstraint() : BinaryConstraint(ValueId(0), ValueId(0), CmpFlags::Default) {}
	        BinaryConstraint(ValueId _first, ValueId _second, CmpFlags _relation);
	bool    Contains        (ValueId id) const { return id == first || id == second; }
	ValueId GetOther        (ValueId id) const
  {
    if (id == first) 
      return second; 
    else if (id == second) 
      return first; 
    else throw std::runtime_error("Constraint does not contain ValueId it should");
  }
};


typedef uint64_t ConstraintId;

class ValueContainer : public IValueContainer
{
private:
	std::map<ValueId, uint64_t> constantContainer; //boost bimap?				//contains mapping from ValueId to constant
	std::map<ConstraintId, BinaryConstraint> constrContainer;			//contains mapping from ConstraintId to Constraint
	std::map< ValueId, std::vector<ConstraintId>> constrIdContainer;	//contains mapping from ValueId to all used ConstraintIds

	static ConstraintId nextConstraintIdToGive; //can be static?
	static ConstraintId GetNextConstraintId() { return nextConstraintIdToGive++; }
	void InsertConstraint(BinaryConstraint);
	void DeleteConstraint(ConstraintId constrId);
	const std::vector<ConstraintId> &GetConstraintIdVector(const ValueId id) const;


public:

	ValueContainer() : constantContainer(), constrContainer(), constrIdContainer(){}
	ValueContainer(const ValueContainer &c) : constantContainer(c.constantContainer), constrContainer(c.constrContainer), constrIdContainer(c.constrIdContainer){}
	bool IsConstant(ValueId first) const { auto res = constantContainer.find(first); return res != constantContainer.end(); }

	boost::tribool IsCmp(ValueId first, ValueId second, Type type, CmpFlags flags) const override;
	boost::tribool IsEq(ValueId first, ValueId second, Type type) const override;
	boost::tribool IsNeq(ValueId first, ValueId second, Type type) const override;
	boost::tribool IsTrue(ValueId first, Type type) const override; // !=0
	boost::tribool IsFalse(ValueId first, Type type) const override; // ==0

	boost::tribool IsBinaryEq(ValueId first, ValueId second) const override;
	boost::tribool IsUnknown(ValueId first) const override;
	boost::tribool IsZero(ValueId first) const override;

	void    Assume(ValueId first, ValueId second, Type type, CmpFlags flags) override;
	void    AssumeTrue(ValueId first) override; // Sets contraint: first != 0 ( == true )
	void    AssumeFalse(ValueId first) override; // Sets contraint: first == 0 ( == false)

	ValueId Add(ValueId first, ValueId second, Type type, ArithFlags flags) override;
	ValueId Sub(ValueId first, ValueId second, Type type, ArithFlags flags) override;
	ValueId Mul(ValueId first, ValueId second, Type type, ArithFlags flags) override;
	ValueId Div(ValueId first, ValueId second, Type type, ArithFlags flags) override;
	ValueId Rem(ValueId first, ValueId second, Type type, ArithFlags flags) override;

	ValueId LSh(ValueId first, ValueId second, Type type, ArithFlags flags) override;
	ValueId RSh(ValueId first, ValueId second, Type type, ArithFlags flags) override;

	ValueId BitAnd(ValueId first, ValueId second, Type type) override;
	ValueId BitOr(ValueId first, ValueId second, Type type) override;
	ValueId BitXor(ValueId first, ValueId second, Type type) override;
	ValueId BitNot(ValueId first, Type type) override;

	//following are not an LLVM operations
	//when C/C++ is compiled in LLVM, everything is series of !override tests and branch/phi instructions
	ValueId LogAnd(ValueId first, ValueId second, Type type) override;
	ValueId LogOr(ValueId first, ValueId second, Type type) override;
	ValueId LogNot(ValueId first, Type type) override;

	ValueId ExtendInt(ValueId first, Type sourceType, Type targetType, ArithFlags flags) override;
	ValueId TruncInt(ValueId first, Type sourceType, Type targetType) override;

	//ValueId ConvIntToFloat(ValueId first, uint32_t flags) override;
	//ValueId ConvFloatToInt(ValueId first, uint32_t flags) override;

	ValueId CreateVal(Type type) override { return ValueId::GetNextId(); }

	ValueId CreateConstIntVal(uint64_t value, Type type) override;
	ValueId CreateConstIntVal(uint64_t value) override; // To be potentially removed
	ValueId CreateConstFloatVal(float    value, Type type) override;
	ValueId CreateConstFloatVal(double   value, Type type) override;

};

inline uint64_t RipBits(size_t numOfBits, uint64_t in)
{
	in &= (1ULL << numOfBits) - 1;
	return in;
}

inline uint64_t SignExtend64(size_t numOfBits, uint64_t in)
{
		in = RipBits(numOfBits, in);
		uint64_t mask = (1ULL << (numOfBits - 1));
		return  (uint64_t) (-((int64_t)(in & mask))) | in;
}

//bit scan reverse - returns position of first set bit from the left side
inline uint64_t BSR(uint64_t val)
{
	uint64_t pos = 0;
	while (val >>= 1)
		pos++;
	return pos;
}