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
void	add(const DynamicMath &main, const DynamicMath &add, size_t oversize);

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
friend DynamicMath operator+(DynamicMath tmp, const DynamicMath &op); \
DynamicMath &operator-(const DynamicMath &op); \
DynamicMath &subtract(const DynamicMath &op, bool revOp); \
void	addition(const DynamicMath &op, bool revOp);

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
		size_t	oversize0Number() const;
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
			(data)[i] = ((unsigned char *)(&t))[size - i - 1];
	else
		memcpy(data, &t, size);
}

#endif