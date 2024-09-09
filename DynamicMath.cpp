#include "DynamicMath.hpp"
#include "DynamicUtils.hpp"

static void getSign(std::string &str, bool &negative)
{
	if (str[0] == '-')
	{
		negative = true;
		str.erase(0, 1);
	}
	else if (str[0] == '+')
		str.erase(0, 1);
}

static void	parse(std::string &str, uint64_t &comma, bool &negative, bool &decimal)
{
	getSign(str, negative);
	std::regex reg(R"(\d+\.?\d*)");
	if (!std::regex_match(str, reg))
		throw std::invalid_argument("Argument doesn't fit the following charset: \"0-9\" and \".\"");
	
	std::string::iterator it;
	it = std::find_if(str.begin(), str.end(), [](char c){return c != '0';});
	if (it == str.end() && str.size() > 1)
	{
		str = "0";
		return;
	}
	else
		str.erase(str.begin(), it);
	if ((it = std::find(str.begin(), str.end(), '.')) != str.end())
	{
		comma = it - str.begin();
		size_t i = str.find_last_not_of('0');
		if (i != str.size() && i != str.npos && i > comma)
			str.erase(i+1);
		else if (i == comma)
		{
			str.erase(i);
			return;
		}
		decimal = true;
		comma = str.end() - it - 1;
		str.erase(it);
	}
}

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

void	DynamicMath::parseString(const std::string str)
{
	std::string newstr = str;
	resetValues();
	parse(newstr, comma, negative, decimal);
	size_t len = newstr.size();
	size = roundUp(floor(len / LOG10OF2) + 1, 8) / 8; //log10(number) / log10(2) = number of bits to allocate

	allocData(data, size);

	uint64_t extrabits = size * 8;
	for (;newstr.size(); extrabits--)
		charToBinary(newstr, data, size);

	for (;extrabits > 0; extrabits--)
		memshift(data, size);
}


void	DynamicMath::printData()
{
	std::cout << "size: " << size << std::endl;
	std::cout << "comma: " << comma << std::endl;
	std::cout << "decimal: " << decimal << std::endl;
	std::cout << "negative: " << negative << std::endl;
	printBinary(data, size);
	std::cout << std::endl;
}

DynamicMath &DynamicMath::operator=(const DynamicMath &cp)
{
	resetValues();
	negative = cp.negative;
	decimal = cp.decimal;
	size = cp.size;
	comma = cp.comma;
	allocData(data, size);
	memcpy(data, cp.data, size);
	return *this;
}

std::ostream& operator<<(std::ostream& os, const DynamicMath& obj)
{
	(void)obj;
	return os;
}

DynamicMath::DynamicMath()
{
	parseIntegral(0);
}

DynamicMath::DynamicMath(const DynamicMath &cp)
{
	*this = cp;
}

DynamicMath::~DynamicMath()
{
	if (data)
		free(data);
}