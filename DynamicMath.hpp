#ifndef DYNAMICMATH_HPP
# define DYNAMICMATH_HPP
# include <string>
# include <exception>
# include <iostream>
# include <cstring>
# include <bitset>
# include <regex>
# include <type_traits>

# define LOG10OF2 0.30102999566398114

# define UTILS \
void	resetValues(); \
void	mutiplicationBuffer(const DynamicMath &op); \
void	add(const DynamicMath &op); \
void	multiply(const DynamicMath &op); \

# define CONSTRUCTORS \
DynamicMath(); \
~DynamicMath(); \
DynamicMath(const std::string &str) {parseString(str);}; \
DynamicMath(const char *str) {parseString(str);}; \
DynamicMath(const short &val) {parseIntegral(val);}; \
DynamicMath(const unsigned short &val) {parseIntegral(val);}; \
DynamicMath(const int &val) {parseIntegral(val);}; \
DynamicMath(const unsigned int &val) {parseIntegral(val);}; \
DynamicMath(const long &val) {parseIntegral(val);}; \
DynamicMath(const unsigned long &val) {parseIntegral(val);}; \
DynamicMath(const long long &val) {parseIntegral(val);}; \
DynamicMath(const unsigned long long &val) {parseIntegral(val);}; \
DynamicMath(const DynamicMath &cp);

# define ASSIGN_OPERATOR \
DynamicMath &operator=(const std::string &str) {parseString(str); return *this;}; \
DynamicMath &operator=(const char *str) {parseString(str); return *this;}; \
DynamicMath &operator=(const short &val) {parseIntegral(val); return *this;}; \
DynamicMath &operator=(const unsigned short &val) {parseIntegral(val); return *this;}; \
DynamicMath &operator=(const int &val) {parseIntegral(val); return *this;}; \
DynamicMath &operator=(const unsigned int &val) {parseIntegral(val); return *this;}; \
DynamicMath &operator=(const long &val) {parseIntegral(val); return *this;}; \
DynamicMath &operator=(const unsigned long &val) {parseIntegral(val); return *this;}; \
DynamicMath &operator=(const long long &val) {parseIntegral(val); return *this;}; \
DynamicMath &operator=(const unsigned long long &val) {parseIntegral(val); return *this;}; \
DynamicMath &operator=(const DynamicMath &cp);

# define OPERATIONS \
DynamicMath &operator+=(const DynamicMath &op); \
DynamicMath &operator-=(const DynamicMath &op); \
DynamicMath &operator*=(const DynamicMath &op); \
friend DynamicMath operator+(DynamicMath tmp, const DynamicMath &op); \
friend DynamicMath operator-(DynamicMath tmp, const DynamicMath &op); \
friend DynamicMath operator*(DynamicMath tmp, const DynamicMath &op); \

class DynamicMath
{
	private:
		bool			negative = false;
		bool			decimal = false;
		size_t			size = 0;
		uint64_t		comma = 0;
		unsigned char	*data = NULL;
		UTILS
		template<typename T>
		void	parseIntegral(const T &t);
		void	parseString(const std::string str);
	public:
		CONSTRUCTORS
		ASSIGN_OPERATOR
		OPERATIONS
		void	printData();
};

std::ostream& operator<<(std::ostream& os, const DynamicMath& obj);

template<typename T>
void	DynamicMath::parseIntegral(const T &t)
{
	resetValues();
	if (t < 0)
		negative = true;
	size = sizeof(t);
	if (data)
		free(data);
	data = (unsigned char *)malloc(size);
	if (!data)
		throw std::bad_alloc();
	bzero(data, size);
	int n = 1;
	// little endian if true
	if(*(char *)&n == 1)
		for (size_t i = 0; i < size; i++)
			data[i] = ((unsigned char *)(&t))[size - i - 1];
	else
		memcpy(data, &t, size);
	if (negative)
	{
		for (size_t i = 0; i < size; i++)
			data[i] = ~data[i];
		unsigned char buff = 0;
		unsigned char add_to_buff = 1;
		size_t i = size - 1;
		if (i != SIZE_MAX)
		{
			for (int j = 0; j < 8; j++)
			{
				char nextBuff = 0;
				if ((data[i] >> j) % 2 && (add_to_buff >> j) % 2)
					nextBuff = 1;
				else if (((data[i] >> j) % 2 || (add_to_buff >> j) % 2) && buff)
					nextBuff = 1;
				char add = ((data[i] >> j) % 2 != (add_to_buff >> j) % 2) != buff;
				if ((data[i] >> j) % 2)
					data[i] -= 1 << j;
				if (add)
					data[i] += add << j;
				buff = nextBuff;
			}
			i--;
		}
		add_to_buff = 0;
		for (; i != SIZE_MAX && buff; i--)
		{
			for (int j = 0; j < 8; j++)
			{
				char nextBuff = 0;
				if ((data[i] >> j) % 2 && (add_to_buff >> j) % 2)
					nextBuff = 1;
				else if (((data[i] >> j) % 2 || (add_to_buff >> j) % 2) && buff)
					nextBuff = 1;
				char add = ((data[i] >> j) % 2 != (add_to_buff >> j) % 2) != buff;
				if ((data[i] >> j) % 2)
					data[i] -= 1 << j;
				if (add)
					data[i] += add << j;
				buff = nextBuff;
			}
		}
		if (i == SIZE_MAX && buff)
		{
			size_t newsize = size + 1;
			unsigned char *newdata = (unsigned char *)malloc(newsize);
			if (!newdata)
				throw std::bad_alloc();
			bzero(newdata, newsize);
			memcpy(newdata + newsize - size, data, size);
			free(data);
			data = newdata;
			size = newsize;
			data[0] += 1;
		}
	}
}

#endif