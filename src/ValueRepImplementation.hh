//
// Created by Charvin on 17. 10. 2016.
//

#ifndef _VALUEREPIMPLEMENTATION_HH_
#define _VALUEREPIMPLEMENTATION_HH_

#include "Values.hh"
#include "Type.hh"

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <ostream>

typedef uint64_t ValueId;

/*
 * Typedef union for constants in interval value representations
 */
typedef union{
	int64_t Integer;
}IntervalData;

/*
 * Class representing Binary relation constraint
 * \detail idVal1 type idVal2 (e.g. 42 < 1337 ... Value representation of id 42 is less than value representation of id 1337)
 */
class BinaryConstraint{
private:
	static ValueId  constraintIdGenerator;

public:
	ValueId id;
	ValueId idVal1;
	ValueId idVal2;
	ConstraintType type;

	BinaryConstraint(ValueId _idVal1, ConstraintType _type, ValueId _idVal2) : idVal1(_idVal1), idVal2(_idVal2), type(_type), id(constraintIdGenerator++) {}
	friend std::ostream& operator<<(std::ostream& stream, BinaryConstraint val);
};

/*
 * Class representing binary relations
 * \details ValueRep1 has IntervalConstraint of type eGT and data 42 -> ValueRep1 > 42
 */
class IntervalConstraint{
public:
	ConstraintType type;
	IntervalData   data;

	IntervalConstraint(ConstraintType type, int64_t val) : type(type) {data.Integer = val;} ///if IntervalData changes (maybe add float?..) overload constructor is all it takes
	friend std::ostream& operator<< (std::ostream& stream, IntervalConstraint val);
};


/*
 * Class implementing ValueRepresentation
 */
class ValueRep : public IValueRep
{
private:
	static ValueId  valueIdGenerator;
	static std::unordered_map < ValueId , std::pair < IType , std::vector< ValueId > > >   binaryRelationsContainer;    ///hash table of [ValueRep id, pair[ValueRep type , vector[ Constraint id ]]]
	static std::unordered_map < ValueId , BinaryConstraint >                               binaryConstraintContainer;   ///hash table of [Constraint id, Binary constraint]
	static std::unordered_map < ValueId , std::vector< IntervalConstraint >  >             intervalConstraintContainer; ///hash table of [ValueRep id, vector[IntervalConstraint]]
	//those names tho...

	ValueId id;

public:
	ValueRep() : id(valueIdGenerator++) {} //ctr
	virtual ~ValueRep() override;

	virtual std::string ToString() const override;
	virtual boost::tribool operator<(const IValueRep &other) const override;
	virtual boost::tribool operator>(const IValueRep &other) const override;
	virtual boost::tribool operator<=(const IValueRep &other) const override;
	virtual boost::tribool operator>=(const IValueRep &other) const override;
	virtual boost::tribool IsEQ(const IValueRep &other) const override;
	virtual boost::tribool IsNEQ(const IValueRep &other) const override;

	virtual boost::tribool operator<(const int64_t constValue) const override;
	virtual boost::tribool operator>(const int64_t constValue) const override;
	virtual boost::tribool operator<=(const int64_t constValue) const override;
	virtual boost::tribool operator>=(const int64_t constValue) const override;
	virtual boost::tribool IsEQ(const int64_t constValue) const override;
	virtual boost::tribool IsNEQ(const int64_t constValue) const override;
	virtual boost::tribool IsUnknown() const override;
	virtual boost::tribool IsZero() const override {return IsEQ(0);}

	virtual void SetConstraint(ConstraintType type, IValueRep &other) override;
	virtual void SetConstraint(ConstraintType type, int64_t constValue) override;

	friend std::ostream& operator<< (std::ostream& stream, IValueRep val) const { return stream << val.ToString(); }
};



#endif //_VALUEREPIMPLEMENTATION_HH_
