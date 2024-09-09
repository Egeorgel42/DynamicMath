#include "DynamicMath.hpp"
#include "DynamicUtils.hpp"

void	DynamicMath::add(const DynamicMath &op)
{
	uint64_t delta_comma = comma > op.comma ? comma - op.comma : op.comma - comma; //abs of main - op.coma
	unsigned char *buff = NULL;
	size_t buff_size = op.size;

	if (comma > op.comma || op.negative)
	{
		if (negative && op.negative && oversize0Number(op.data, op.size) == 0)
			buff_size++;
		allocData(buff, buff_size);
		memcpy(buff + buff_size - op.size, op.data, op.size);
		if (comma > op.comma)
			resizeToComma(buff, buff_size, delta_comma, oversize0Number(buff, buff_size));
		if (op.negative)
			complement(buff, buff_size);
	}
	else
		buff = op.data;
	if (comma < op.comma || negative)
	{
		if (negative && op.negative && oversize0Number(data, size) == 0)
			reallocData(data, size, size + 1);
		if (comma < op.comma)
		{
			resizeToComma(data, size, delta_comma, oversize0Number(data, size));
			comma = op.comma;
		}
		if (negative)
			complement(data, size);
	}
	if (size < buff_size)
		reallocData(data, size, buff_size);
	bool res = addToBuffer(data, size, buff, buff_size, negative || op.negative, op.negative);
	if ((!res && negative != op.negative) || (res && negative && op.negative))
	{
		complement(data, size);
		negative = true;
	}
	else if (!res && negative && op.negative)
		throw std::logic_error("expected for carry over in operation");
	else
		negative = false;
	if (buff != op.data)
		free(buff);
}

DynamicMath &DynamicMath::operator+=(const DynamicMath &op)
{
	decimal = decimal || op.decimal;
	add(op);
	return *this;
}

DynamicMath &DynamicMath::operator-=(const DynamicMath &op)
{
	decimal = decimal || op.decimal;
	negative = !negative;
	add(op);
	negative = !negative;
	return *this;
}

DynamicMath operator+(DynamicMath tmp, const DynamicMath &op)
{
	tmp += op;
	return tmp;
}

DynamicMath operator-(DynamicMath tmp, const DynamicMath &op)
{
	tmp -= op;
	return tmp;
}