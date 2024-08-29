#include <string>
#include <exception>
#include <iostream>
#include <cstring>
#include <bitset>

#define LOG10OF2 0.30102999566398114

class DynamicMath
{
	private:
		bool	negative = false;
		size_t	size;
		unsigned char	*data;
	public:
		DynamicMath(char *str);
		DynamicMath(std::string str);
		~DynamicMath();
};
