#include "DynamicMath.hpp"

size_t	oversize0Number(unsigned char *data, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		if (data[i] == 0)
			continue;
		for (short j = 7; j >=0; j--)
		{
			if ((data[i] >> j) % 2)
				return i*8 + 7-j;
		}
	}
	return size;
}

char	binaryAddition(unsigned char &dest, unsigned char &op, char buff)
{
	for (int i = 0; i < 8; i++)
	{
		char nextBuff = 0;
		if ((dest >> i) % 2 && (op >> i) % 2)
			nextBuff = 1;
		else if (((dest >> i) % 2 || (op >> i) % 2) && buff)
			nextBuff = 1;
		char add = ((dest >> i) % 2 != (op >> i) % 2) != buff;
		if ((dest >> i) % 2)
			dest -= 1 << i;
		if (add)
			dest += add << i;
		buff = nextBuff;
	}
	return buff;
}

/// @brief a memcpy that reverses all 0 and 1s, this function is made to enable substractions, for more details lookup addtion/substraction using 1's complement
void	complement_memcpy(void *dest, const void *src, const size_t size)
{
	for (size_t i = 0; i < size; i++)
		((unsigned char *)dest)[i] = ~((unsigned char *)src)[i];
}

/// @brief reverses all 0 and 1s, this function is made to enable substractions, for more details lookup addtion/substraction using 1's complement
void	complement(void *dest, const size_t size)
{
	for (size_t i = 0; i < size; i++)
		((unsigned char *)dest)[i] = ~((unsigned char *)dest)[i];
}


void allocData(unsigned char *&data, size_t size)
{
	if (data)
		free(data);
	data = (unsigned char *)malloc(size);
	if (!data)
		throw std::bad_alloc();
	bzero(data, size);
}

void	reallocData(unsigned char *&data, size_t &size, size_t newsize)
{
	if (newsize <= size)
		return;
	unsigned char *newdata = (unsigned char *)malloc(newsize);
	if (!newdata)
		throw std::bad_alloc();
	bzero(newdata, newsize);
	memcpy(newdata + newsize - size, data, size);
	free(data);
	data = newdata;
	size = newsize;
}
/// @brief Will return true if the result of the operation resulted in a "carry over" when has_negative = true, reverse the numbers when only one of the numbers was negative and the return was negative or when both of the numbers are negative (should always return true, if not something is wrong)
bool	addToBuffer(unsigned char *data, size_t &size, unsigned char *add, size_t add_size, bool has_negative, bool fill_with_1)
{
	unsigned char buff = 0;
	unsigned char null = 0;
	if (fill_with_1 && has_negative)
		null = 255;
	if (add_size > size)
		reallocData(data, size, add_size);
	size_t i = size - 1;
	size_t j = add_size - 1;
	for (; i != SIZE_MAX && j != SIZE_MAX; i--, j--)
		buff = binaryAddition(data[i], add[j], buff);
	for (; i != SIZE_MAX && (buff || null != 0); i--)
		buff = binaryAddition(data[i], null, buff);
	if (i == SIZE_MAX && buff && has_negative)
	{
		addToBuffer(data, size, &buff, 1, false, false);
		return true;
	}
	if (i == SIZE_MAX && buff)
	{
		reallocData(data, size, size + 1);
		data[0] += 1;
	}
	return false;
}

void	memshift(unsigned char *buf, size_t len)
{
	bool mem = false;
	bool next_mem = false;
	for (size_t i = 0; i < len; i++, mem = next_mem, next_mem = false)
	{
		if (buf[i] % 2)
			next_mem = true;
		buf[i] >>= 1;
		if (mem)
			buf[i] += 128;
	}
}

void	memshiftL(unsigned char *data, size_t size, size_t shiftSize)
{
	if (!shiftSize)
		return;
	bool *mem = new bool[shiftSize];
	bool *next_mem = new bool[shiftSize];
	memset(mem, (int)false, shiftSize);
	memset(next_mem, (int)false, shiftSize);
	size--;
	for (; size != SIZE_MAX; size--)
	{
		memcpy(mem, next_mem, shiftSize);
		memset(next_mem, (int)false, shiftSize);
		for (size_t j = 0; j < shiftSize; j++)
		{
			if (data[size] / 128)
				next_mem[j] = true;
			data[size] <<= 1;
			if (mem[j])
				data[size] += 1;
		}
	}
	delete[] mem;
	delete[] next_mem;
}

void charToBinary(std::string &str, unsigned char *data, uint64_t size)
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

void	printBinary(unsigned char *data, size_t len)
{
	std::bitset<8> x;
	std::cout << "data: ";
	for (size_t i = 0; i < len; i++)
	{
		x = data[i];
		std::cout << x;
	}
	std::cout << std::endl;
}

size_t floor(long double nbr)
{
	size_t buf = (size_t) nbr;
	if ((long double) buf == nbr || nbr >= 0)
		return buf;
	return buf - 1;
}

size_t roundUp(size_t numToRound, size_t multiple)
{
    if (multiple == 0)
        return numToRound;

    size_t remainder = numToRound % multiple;
    if (remainder == 0)
        return numToRound;

    return numToRound + multiple - remainder;
}
