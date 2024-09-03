#include "DynamicMath.hpp"

void	revmemcpy(void *dest, const void *src, const size_t size)
{
	for (size_t i = 0; i < size; i++)
		((char *) dest)[i] = ((char *)src)[size - i - 1];
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
