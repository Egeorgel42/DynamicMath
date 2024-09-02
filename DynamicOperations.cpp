#include "DynamicMath.hpp"
#include "DynamicUtils.hpp"

static char	binaryAddition(unsigned char &dest, unsigned char &op, char buff)
{
	for (int i = 0; i < 8; i++)
	{
		char nextBuff = 0;
		if ((dest >> i) % 2 && (op >> i) % 2)
			nextBuff = 1;
		else if (((dest >> i) % 2 || (op >> i) % 2) && buff)
			nextBuff = 1;
		char add = ((dest >> i) % 2 != (op >> i) % 2) + buff;
		if ((dest >> i) % 2)
			dest -= 1 << i;
		if (add)
			dest += add << i;
		buff = nextBuff;
	}
	return buff;
}

void	addToBuffer(unsigned char *data, size_t &size, unsigned char *add, size_t add_size)
{
	char buff = 0;
	unsigned char null = 0;
	if (add_size > size)
		reallocData(data, size, add_size);
	size_t i = size - 1;
	size_t j = add_size - 1;
	for (; i != SIZE_MAX && j != SIZE_MAX; i--, j--)
		buff = binaryAddition(data[i], add[j], buff);
	for (; i != SIZE_MAX && buff; i--)
		buff = binaryAddition(data[i], null, buff);
	if (i == SIZE_MAX && buff)
	{
		reallocData(data, size, size + 1);
		data[0] += 1;
	}
}

DynamicMath &DynamicMath::subtract(const DynamicMath &op, bool revOp)
{
	(void)op;
	(void)revOp;
	return *this;
}

void	DynamicMath::add(const DynamicMath &main, const DynamicMath &add, size_t oversize)
{
	uint64_t delta_comma = main.comma > add.comma ? main.comma - add.comma : add.comma - main.comma; //abs of main - op.coma
	unsigned char *buff = NULL;
	size_t newsize = size;

	comma = add.comma;
	if (delta_comma)
	{
		size_t added_size = floor(delta_comma / LOG10OF2) + 1;
		if (added_size > oversize)
			newsize += roundUp(added_size - oversize, 8) / 8;
	}
	allocData(buff, newsize);
	memcpy(buff + newsize - main.size, main.data, main.size);
	if (delta_comma)
	{
		memshiftL(buff, newsize, 2 * delta_comma);
		addToBuffer(buff, newsize, main.data, main.size);
		memshiftL(buff, newsize, 1 * delta_comma);
	}
	addToBuffer(buff, newsize, add.data, add.size);
	size = newsize;
	free(data);
	data = buff;
}

void	DynamicMath::addition(const DynamicMath &op, bool revOp)
{
	if (negative != op.negative && !revOp)
		subtract(op, true);
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
}

DynamicMath &DynamicMath::operator+=(const DynamicMath &op)
{
	addition(op, false);
	return *this;
}

DynamicMath operator+(DynamicMath tmp, const DynamicMath &op)
{
	tmp += op;
	return tmp;
}