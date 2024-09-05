#include "DynamicMath.hpp"
#include "DynamicUtils.hpp"

void	DynamicMath::additionBuffer(const DynamicMath &op, unsigned char *&buff, size_t &current_size)
{
	if (op.size > size)
		reallocData(data, size, op.size);
	if (comma == op.comma)
	{
		allocData(buff, size);
		memcpy(buff, data, size);
		current_size = size;
		return;
	}
	uint64_t delta_comma = comma > op.comma ? comma - op.comma : op.comma - comma; //abs of main - op.coma
	unsigned char *buff_add = NULL;
	current_size = size;
	size_t buff_size = current_size;
	size_t oversize = oversize0Number(data, size);

	if (size == op.size)
		oversize = oversize < oversize0Number(op.data, op.size) ? oversize : oversize0Number(op.data, op.size);
	else if (op.size > size)
		oversize = oversize0Number(op.data, op.size);

	size_t added_size = floor(delta_comma / LOG10OF2) + 1;
	if (added_size > oversize)
		current_size += roundUp(added_size - oversize, 8) / 8;

	allocData(buff, current_size);
	allocData(buff_add, buff_size);
	if (comma > op.comma)
		memcpy(buff + current_size - size, data, size);
	else
		memcpy(buff + current_size - op.size, op.data, op.size);
	for (; delta_comma > 0; delta_comma--)
	{
		if (buff_size != current_size)
			reallocData(buff_add, buff_size, current_size);
		memcpy(buff_add, buff, buff_size);
		memshiftL(buff, current_size, 3);
		memshiftL(buff_add, current_size, 1);
		addToBuffer(buff, current_size, buff_add, buff_size, false, false);
	}
	comma = comma > op.comma ? comma : op.comma;
	free(buff_add);
}

void	DynamicMath::add(const DynamicMath &op)
{
	unsigned char *buff = NULL;
	unsigned char *buff_add = NULL;
	size_t buff_size;
	size_t add_size = op.size;

	additionBuffer(op, buff, buff_size);
	if (negative && op.negative && oversize0Number(buff, buff_size) == 0)
		reallocData(buff, buff_size, buff_size + 1);
	if (negative && op.negative && oversize0Number(op.data, op.size) == 0)
		add_size++;
	if (negative)
		complement(buff, buff_size);
	allocData(buff_add, add_size);
	memcpy(buff_add + add_size - op.size, op.data, op.size);
	if (op.negative)
		complement(buff_add, add_size);
	bool res = addToBuffer(buff, buff_size, buff_add, add_size, negative || op.negative, op.negative);
	if ((!res && negative != op.negative) || (res && negative && op.negative))
	{
		complement(buff, buff_size);
		negative = true;
	}
	else if (!res && negative && op.negative)
		throw std::logic_error("expected for carry over in operation");
	else
		negative = false;
	size = buff_size;
	free(buff_add);
	free(data);
	data = buff;
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