#include "DynamicMath.hpp"
#include "DynamicUtils.hpp"

void	DynamicMath::resetValues()
{
	negative = false;
	decimal = false;
	size = 0;
	comma = 0;
	if (data)
		free(data);
	data = NULL;
}

std::string	DynamicMath::returnData()
{
	std::string res;
	res = "size: " + std::to_string(size) + "\n";
	res += "comma: " + std::to_string(comma) + "\n";
	res += "decimal: " + std::to_string(decimal) + "\n";
	res += "negative: " + std::to_string(negative) + "\n";
	res += returnBinary(data, size);
	res += this->toString();
	res += "\n";
	return res;
}


DynamicMath		DynamicMath::precision()
{
	DynamicMath temp = size;
	DynamicMath Log2of10 = "0.301029995663981195213738894724";

	temp *= 8;
	temp -= oversize0Number(data, size);
	temp = temp * Log2of10 + 1;

	return temp;
}

DynamicMath DynamicMath::operator--(int null)
{
	(void)null;
	return *this -= 1;
}

DynamicMath DynamicMath::operator++(int null)
{
	(void)null;
	return *this += 1;
}

DynamicMath &DynamicMath::operator--()
{
	return *this -= 1;
}

DynamicMath &DynamicMath::operator++()
{
	return *this += 1;
}

/// @brief used for ceil/floor to divide by the number of trailling decimal after the comma
/// @param div is equal to 10^X
/// @return a new DynamicMath object
DynamicMath DynamicMath::divNoRem(DynamicMath div, bool &pureDividend) const
{
	if (diff(div.data, div.size, this->data, this->size) == -1)
		return 0;

	DynamicMath buff = *this;
	DynamicMath res = 0;

	DynamicMath divSizeBuff = div.size;
	divSizeBuff *= 8;
	divSizeBuff -= oversize0Number(div.data, div.size);

	DynamicMath sizeBuff = size;
	sizeBuff *= 8;
	sizeBuff -= oversize0Number(data, size);

	reallocData(res.data, res.size, size);
	reallocData(div.data, div.size, size);
	for (DynamicMath i = sizeBuff - divSizeBuff; i > 0; i--) //horrible perfomance but im short on patience
		memshiftL(div.data, div.size, 1);
	buff.comma = 0;
	for (DynamicMath i = sizeBuff; i >= divSizeBuff; i--)
	{
		if (diff(buff.data, buff.size, div.data, div.size) == 1)
			memshiftL(res.data, res.size, 1);
		else
		{
			memshiftL(res.data, res.size, 1);
			res++;
			buff -= div;
		}
		memshiftR(div.data, div.size, 1);
	}
	if (buff == 0)
		pureDividend = true;
	return res;
}

DynamicMath	DynamicMath::dCeil() const
{
	if (!decimal)
		return *this;
	DynamicMath res;
	DynamicMath divSize = 10;
	bool pureDiv = false;

	divSize = divSize^comma;
	res = this->divNoRem(divSize, pureDiv);
	if (!pureDiv)
		res++;
	return res;
}

DynamicMath	DynamicMath::dFloor() const
{
	if (!decimal)
		return *this;
	DynamicMath res;
	DynamicMath divSize = 10;
	bool pureDiv = false;

	divSize = divSize^comma;
	return this->divNoRem(divSize, pureDiv);
}