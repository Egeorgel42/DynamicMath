#include "DynamicMath.hpp"
#include "DynamicUtils.hpp"

void	DynamicMath::add(const DynamicMath &main, const DynamicMath &add, size_t oversize)
{
	uint64_t delta_comma = main.comma > add.comma ? main.comma - add.comma : add.comma - main.comma; //abs of main - op.coma
	unsigned char *buff = NULL;
	unsigned char *buff_add = NULL;
	size_t buff_size;
	size_t newsize = size;

	comma = add.comma;
	if (delta_comma)
	{
		size_t added_size = floor(delta_comma / LOG10OF2) + 1;
		if (added_size > oversize)
			newsize += roundUp(added_size - oversize, 8) / 8;
	}
	buff_size = newsize;
	allocData(buff, newsize);
	allocData(buff_add, buff_size);
	memcpy(buff + newsize - main.size, main.data, main.size);
	for (; delta_comma > 0; delta_comma--)
	{
		if (buff_size != newsize)
			reallocData(buff_add, buff_size, newsize);
		memcpy(buff_add, buff, buff_size);
		memshiftL(buff, newsize, 3);
		memshiftL(buff_add, newsize, 1);
		addToBuffer(buff, newsize, buff_add, newsize, negative);
	}
	if (buff_size != newsize)
		reallocData(buff_add, buff_size, newsize);
	if (main.negative && add.negative && (main.oversize0Number() == 0 || add.oversize0Number() == 0))
	{
		reallocData(buff, newsize, newsize + 1);
		reallocData(buff_add, buff_size, newsize);
	}
	if (main.negative)
		complement(buff, newsize);
	bzero(buff_add, buff_size);
	memcpy(buff_add + buff_size - add.size, add.data, add.size);
	if (add.negative)
		complement(buff_add, buff_size);
	printBinary(buff_add, buff_size);
	printBinary(buff, newsize);
	bool res = addToBuffer(buff, newsize, buff_add, buff_size, main.negative || add.negative);
	if ((!res && main.negative != add.negative) || (res && main.negative && add.negative))
	{
		complement(buff, newsize);
		negative = true;
	}
	else if (!res && main.negative && add.negative)
		throw std::logic_error("expected for carry over in operation");
	else
		negative = false;
	size = newsize;
	free(buff_add);
	free(data);
	data = buff;
}

DynamicMath &DynamicMath::operator+=(const DynamicMath &op)
{
	decimal = decimal || op.decimal;
	size_t oversize = oversize0Number();

	if (size == op.size)
		oversize = oversize < op.oversize0Number() ? oversize : op.oversize0Number();
	if (op.size > size)
	{
		reallocData(data, size, op.size);
		oversize = op.oversize0Number();
	}

	if (comma >= op.comma)
		add(op, *this, oversize);
	else
		add(*this, op, oversize);
	return *this;
}

DynamicMath &DynamicMath::operator-=(const DynamicMath &op)
{
	decimal = decimal || op.decimal;
	size_t oversize = oversize0Number();

	if (size == op.size)
		oversize = oversize < op.oversize0Number() ? oversize : op.oversize0Number();
	if (op.size > size)
	{
		reallocData(data, size, op.size);
		oversize = op.oversize0Number();
	}

	negative = !negative;
	if (comma >= op.comma)
		add(op, *this, oversize);
	else
		add(*this, op, oversize);
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