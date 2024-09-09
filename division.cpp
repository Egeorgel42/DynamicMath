#include "DynamicMath.hpp"
#include "DynamicUtils.hpp"


void	DynamicMath::modulo(const DynamicMath &op)
{
	uint64_t delta_comma = comma > op.comma ? comma - op.comma : op.comma - comma; //abs of main - op.coma
	DynamicMath buff;
	size_t zero = 0;

	if (!diff(op.data, op.size, (const unsigned char *)&zero, sizeof(int)))
		throw std::logic_error("Division by 0");

	buff = op;
	buff.negative = false;
	if (size < op.size)
		reallocData(data, size, op.size);
	if (comma < op.comma)
	{
		if (negative && op.negative && oversize0Number(data, size) == 0)
			reallocData(data, size, size + 1);
		if (comma < op.comma)
		{
			resizeToComma(data, size, delta_comma, oversize0Number(data, size));
			comma = op.comma;
		}
	}
	reallocData(buff.data, buff.size, size);
	if (comma > op.comma)
		resizeToComma(buff.data, buff.size, delta_comma, oversize0Number(buff.data, buff.size));

	if (oversize0Number(buff.data, buff.size) >= oversize0Number(data, size))
		memshiftL(buff.data, buff.size, oversize0Number(buff.data, buff.size) - oversize0Number(data, size));
	else
		return;
	while (*this > op)
	{
		DynamicMath res = *this - buff;
		if (res.negative)
		{
			memshift(buff.data, buff.size);
			continue;
		}
		else
			*this = res;
		if (buff > *this)
			memshiftR(buff.data, buff.size, oversize0Number(data, size) - oversize0Number(buff.data, buff.size));
	}
}

DynamicMath &DynamicMath::operator%=(const DynamicMath &op)
{
	negative = false;
	modulo(op);
	decimal = false;
	comma = 0;
	return *this;
}

DynamicMath operator%(DynamicMath tmp, const DynamicMath &op)
{
	tmp %= op;
	return tmp;
}

void	DynamicMath::division(const DynamicMath &op)
{
	uint64_t delta_comma = comma > op.comma ? comma - op.comma : op.comma - comma; //abs of main - op.coma
	unsigned char *buff = NULL;
	size_t buff_size = op.size;
	size_t oversize = oversize0Number(data, size);

	if (size < op.size)
		reallocData(data, size, op.size);
	if (size > op.size)
		oversize = oversize0Number(data, size);
	if (size == op.size)
		oversize = oversize < oversize0Number(op.data, op.size) ? oversize : oversize0Number(op.data, op.size);
	if (comma > op.comma)
	{
		allocData(buff, op.size);
		memcpy(buff, op.data, op.size);
		resizeToComma(buff, buff_size, delta_comma, oversize);
	}
	else
		buff = op.data;
	if (comma < op.comma)
	{
		if (negative && op.negative && oversize0Number(data, size) == 0)
			reallocData(data, size, size + 1);
		if (comma < op.comma)
		{
			resizeToComma(data, size, delta_comma, oversize);
			comma = op.comma;
		}
	}
}