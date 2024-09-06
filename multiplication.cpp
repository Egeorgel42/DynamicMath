#include "DynamicMath.hpp"
#include "DynamicUtils.hpp"

void	DynamicMath::mutiplicationBuffer(const DynamicMath &op)
{
	size_t i = roundUp(size * 8 - oversize0Number(data, size), 8) / 8 + roundUp(op.size * 8 - oversize0Number(op.data, op.size), 8) / 8;
	if (i < roundUp(size * 8 - oversize0Number(data, size), 8) / 8 || i < roundUp(op.size * 8 - oversize0Number(op.data, op.size), 8) / 8)
		throw std::overflow_error("attained maximum computable size");
	reallocData(data, size, i);
}

void	DynamicMath::multiply(const DynamicMath &op)
{
	mutiplicationBuffer(op);
	unsigned char *add_buff = NULL;;
	allocData(add_buff, size);
	memcpy(add_buff, data, size);
	bzero(data, size);
	for(size_t i = op.size - 1; i != SIZE_MAX; i--)
	{
		for (size_t j = 0; j < 8; j++)
		{
			if ((op.data[i] >> j) % 2)
			{
				addToBuffer(data, size, add_buff, size, false, false);
				memshiftL(add_buff, size, 1);
			}
			else
				memshiftL(add_buff, size, 1);
		}
	}
}

DynamicMath &DynamicMath::operator*=(const DynamicMath &op)
{
	decimal = decimal || op.decimal;
	negative = negative != op.negative;
	comma = comma + op.comma;
	multiply(op);
	return *this;
}


DynamicMath operator*(DynamicMath tmp, const DynamicMath &op)
{
	tmp *= op;
	return tmp;
}