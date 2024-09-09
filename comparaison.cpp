#include "DynamicMath.hpp"
#include "DynamicUtils.hpp"


bool operator< (const DynamicMath& lhs, const DynamicMath& rhs) { return diff(lhs.data, lhs.size, rhs.data, rhs.size) == 1 ? true : false; }

bool operator> (const DynamicMath& lhs, const DynamicMath& rhs) { return diff(lhs.data, lhs.size, rhs.data, rhs.size) == -1 ? true : false; }

bool operator<= (const DynamicMath& lhs, const DynamicMath& rhs) { return diff(lhs.data, lhs.size, rhs.data, rhs.size) > -1 ? true : false; }

bool operator>= (const DynamicMath& lhs, const DynamicMath& rhs) { return diff(lhs.data, lhs.size, rhs.data, rhs.size) < 1 ? true : false; }
