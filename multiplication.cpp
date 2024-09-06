#include "DynamicMath.hpp"
#include "DynamicUtils.hpp"

size_t	DynamicMath::mutiplicationSize(const DynamicMath &op)
{
	size_t i = roundUp(size * 8 - oversize0Number(data, size), 8) / 8 + roundUp(op.size * 8 - oversize0Number(op.data, op.size), 8) / 8;
	if (i < size || i < op.size)
		throw std::overflow_error("attained maximum computable size");
	return i;
}

void	DynamicMath::mutiplicationBuffer(const DynamicMath &op, unsigned char *&buff, size_t &current_size)
{
	current_size = mutiplicationSize(op);
	reallocData(data, size, current_size);
	if (comma == op.comma)
	{
		allocData(buff, size);
		memcpy(buff, data, size);
		current_size = size;
		return;
	}
	uint64_t delta_comma = comma > op.comma ? comma - op.comma : op.comma - comma; //abs of main - op.coma
	unsigned char *buff_add = NULL;
	size_t buff_size = current_size;
	size_t oversize = oversize0Number(data, size);

	if (size == op.size)
		oversize = oversize < oversize0Number(op.data, op.size) ? oversize : oversize0Number(op.data, op.size);

	size_t added_size = floor(delta_comma / LOG10OF2) + 1;
	if (added_size > oversize)
	{
		current_size += roundUp(added_size - oversize, 8) / 8;
		if (current_size < added_size)
			throw std::overflow_error("attained maximum computable size");
	}

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

void	DynamicMath::multiply(const DynamicMath &op)
{

}