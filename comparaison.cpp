#include "DynamicMath.hpp"
#include "DynamicUtils.hpp"


bool operator< (const DynamicMath& lhs, const DynamicMath& rhs)
{
	if (lhs.negative == false && rhs.negative == true)
		return false;
	if (lhs.negative == true && rhs.negative == false)
		return true;
	DynamicMath fLhs = lhs.dFloor();
	DynamicMath fRhs = rhs.dFloor();
	int dif = diff(fLhs.data, fLhs.size, fRhs.data, fRhs.size);
	if (dif == 1)
		return true;
	if (dif == -1)
		return false;
	if(diff(lhs.data, lhs.size, rhs.data, rhs.size) == 1)
		return true;
	return false;
}

bool operator> (const DynamicMath& lhs, const DynamicMath& rhs)
{
	if (lhs.negative == true && rhs.negative == false)
		return false;
	if (lhs.negative == false && rhs.negative == true)
		return true;
	DynamicMath fLhs = lhs.dFloor();
	DynamicMath fRhs = rhs.dFloor();
	int dif = diff(fLhs.data, fLhs.size, fRhs.data, fRhs.size);
	if (dif == -1)
		return true;
	if (dif == 1)
		return false;
	if(diff(lhs.data, lhs.size, rhs.data, rhs.size) == 1)
		return true;
	return false;
}

bool operator<= (const DynamicMath& lhs, const DynamicMath& rhs)
{
	if (lhs.negative == false && rhs.negative == true)
		return false;
	if (lhs.negative == true && rhs.negative == false)
		return true;
	DynamicMath fLhs = lhs.dFloor();
	DynamicMath fRhs = rhs.dFloor();
	int dif = diff(fLhs.data, fLhs.size, fRhs.data, fRhs.size);
	if (dif == 1)
		return true;
	if (dif == -1)
		return false;
	if(diff(lhs.data, lhs.size, rhs.data, rhs.size) > -1)
		return true;
	return false;
}

bool operator>= (const DynamicMath& lhs, const DynamicMath& rhs)
{
	if (lhs.negative == true && rhs.negative == false)
		return false;
	if (lhs.negative == false && rhs.negative == true)
		return true;
	DynamicMath fLhs = lhs.dFloor();
	DynamicMath fRhs = rhs.dFloor();
	int dif = diff(fLhs.data, fLhs.size, fRhs.data, fRhs.size);
	if (dif == -1)
		return true;
	if (dif == 1)
		return false;
	if(diff(lhs.data, lhs.size, rhs.data, rhs.size) < 1)
		return true;
	return false;
}

bool operator== (const DynamicMath& lhs, const DynamicMath& rhs)
{
	if(lhs.comma != rhs.comma || lhs.decimal != rhs.decimal || lhs.negative != rhs.negative || diff(lhs.data, lhs.size, rhs.data, rhs.size) != 0)
		return false;
	return true;
}
