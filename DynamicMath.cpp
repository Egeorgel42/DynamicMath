#include "DynamicMath.hpp"

static void	allocBuffer(unsigned char *&allocBuffer, size_t size)
{
	allocBuffer = (unsigned char *)malloc(size);
	if (!allocBuffer)
		throw std::bad_alloc();
	bzero(allocBuffer, size);
}

static void	memshift(unsigned char *buf, size_t len)
{
	bool mem = false;
	bool next_mem = false;
	for (long long i = 0; i < (long long)len; i++, mem = next_mem, next_mem = false)
	{
		if (buf[i] % 2)
			next_mem = true;
		buf[i] >>= 1;
		if (mem)
			buf[i] += 128;
	}
}

static void	printBinary(unsigned char *data, size_t len)
{
	std::bitset<8> x;
	for (size_t i = 0; i < len; i++)
	{
		x = data[i];
		std::cout << x;
	}
	std::cout << std::endl;
}

static size_t floor(long double nbr)
{
	size_t buf = (size_t) nbr;
	if ((long double) buf == nbr || nbr >= 0)
		return buf;
	return buf - 1;
}

static size_t roundUp(size_t numToRound, size_t multiple)
{
    if (multiple == 0)
        return numToRound;

    size_t remainder = numToRound % multiple;
    if (remainder == 0)
        return numToRound;

    return numToRound + multiple - remainder;
}

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

static void charToBinary(std::string &str, unsigned char *data, uint64_t size)
{
	int remainder = 0;
	int next_remainder = 0;
	char c;
	for (size_t i = 0; i < str.size(); i++, remainder = next_remainder, next_remainder = 0)
	{
		c = str[i] - '0';
		if (c % 2)
			next_remainder = 5;
		str[i] = c / 2 + remainder + '0';
	}
	memshift(data, size);
	*data += (c % 2) * 128;
	while (str[0] == '0')
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
		str.erase(it);
	}
}

void	DynamicMath::parseString(std::string str)
{
	parse(str, comma, negative, decimal);
	std::cout << str << std::endl;
	std::cout << comma << std::endl;
	std::cout << decimal << std::endl;
	size_t len = str.size();
	size = roundUp(floor(len / LOG10OF2) + 1, 8) / 8; //log10(number) / log10(2) = number of bits to allocate

	allocBuffer(data, size);

	uint64_t extrabits = size * 8;
	for (;str.size(); extrabits--)
		charToBinary(str, data, size);

	for (;extrabits > 0; extrabits--)
		memshift(data, size);

	printBinary(data, size);
}

DynamicMath::DynamicMath(char *str)
{
	parseString(str);
}

DynamicMath::DynamicMath(std::string str)
{
	parseString(str);
}

DynamicMath::~DynamicMath()
{
	if (data)
		free(data);
}