#pragma once

#include "Definitions.hh"
#include "General.hh"

#include <cstring>

#include <boost/logic/tribool.hpp>

/*
 * Enum class representing Constraint Types - < > <= >= == !=
 */
enum class ConstraintType{
	eLT = 0,
	eGT,
	eLTE,
	eGTE,
	eEQ,
	eNEQ,
};


/*
 * Interface of ValueRepresentation
 */
class IValueRep {
public:

	/*ctr*/ IValueRep(void) {};

	virtual					~IValueRep(void) {};
	virtual std::string		ToString() const = 0;

	virtual boost::tribool	operator<(const IValueRep& other) const = 0;
	virtual boost::tribool	operator>(const IValueRep& other) const = 0;
	virtual boost::tribool	operator<=(const IValueRep& other) const = 0;
	virtual boost::tribool	operator>=(const IValueRep& other) const = 0;
	virtual boost::tribool	IsEQ(const IValueRep& other) const = 0;
	virtual boost::tribool	IsNEQ(const IValueRep& other) const = 0;


	virtual boost::tribool	operator<(const int64_t constValue) const = 0;
	virtual boost::tribool	operator>(const int64_t constValue) const = 0;
	virtual boost::tribool	operator<=(const int64_t constValue) const = 0;
	virtual boost::tribool	operator>=(const int64_t constValue) const = 0;
	virtual boost::tribool	IsEQ(const int64_t constValue) const = 0;
	virtual boost::tribool	IsNEQ(const int64_t constValue) const = 0;
	virtual boost::tribool	IsUnknown() const = 0;
	virtual boost::tribool	IsZero() const = 0;

	virtual void			SetConstraint( ConstraintType type, IValueRep &other) = 0;
	virtual void			SetConstraint( ConstraintType type, int64_t constValue) = 0;

};
