#include "DynamicMath.hpp"
#include "DynamicUtils.hpp"

void	DynamicMath::mutiplicationBuffer(const DynamicMath &op)
{
	size_t newsize = (size + op.size) - (roundDown(oversize0Number(data, size) + oversize0Number(op.data, size), 8) / 8);

	//if newsize is smaller or equal than either the size of either class seperatly then it means size overflowed
	if (size + op.size < size || size + op.size < op.size)
		throw std::overflow_error("attained maximum computable size");
	reallocData(data, size, newsize);
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
	free(add_buff);
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

DynamicMath operator^(DynamicMath tmp, const DynamicMath &op)
{
	if (op.decimal)
		throw std::logic_error("An decimal number as exponent is not supported");
	else if (op.negative)
		throw std::logic_error("An negative number as exponent is not supported");

	DynamicMath mul = tmp;
	tmp = 1;
	for (DynamicMath i = op; i > 0; i--)
		tmp *= mul;
	return tmp;
}