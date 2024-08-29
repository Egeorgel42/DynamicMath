#include "DynamicMath.hpp"

static void	memshift(unsigned char *buf, size_t len)
{
	bool mem = false;
	bool next_mem = false;
	for (long long i = 0; i < (long long)len; i++)
	{
		mem = next_mem;
		next_mem = false;
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

static void getSign(char **str, bool &negative)
{
	if ((*str)[0] == '-')
	{
		negative = true;
		(*str)++;
	}
	else if ((*str)[0] == '+')
		(*str)++;
}

static void charToBinary(char **str, unsigned char *data, uint64_t size)
{
	int rem = 0;
	int next_rem = 0;
	char c;
	for (int i = 0; (*str)[i]; i++)
	{
		rem = next_rem;
		next_rem = 0;
		c = (*str)[i] - '0';
		if (c % 2)
			next_rem = 5;
		(*str)[i] = c / 2 + rem + '0';
	}
	memshift(data, size);
	*data += (c % 2) * 128;
	while (*(*str) == '0')
		(*str)++;
}

DynamicMath::DynamicMath(char *str)
{
	getSign(&str, negative);
	size_t len = strlen(str);
	size = roundUp(floor(len / LOG10OF2) + 1, 8) / 8;
	data = (unsigned char *)malloc(size);
	if (!data)
		throw std::bad_alloc();
	bzero(data, size);
	uint64_t extrabits = size * 8;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			throw std::invalid_argument("charset of string limited between 0 and 9");
		charToBinary(&str, data, size);
		extrabits--;
	}
	while (extrabits > 0)
	{
		memshift(data, size);
		extrabits--;
	}
	printBinary(data, size);
}

DynamicMath::~DynamicMath()
{
	free(data);
}