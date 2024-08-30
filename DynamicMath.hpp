#include <string>
#include <exception>
#include <iostream>
#include <cstring>
#include <bitset>
#include <regex>

#define LOG10OF2 0.30102999566398114

class DynamicMath
{
	private:
		bool			negative = false;
		bool			decimal = false;
		size_t			size;
		uint64_t		comma = 0;
		unsigned char	*data = NULL;
		void	parseString(std::string str);
	public:
		DynamicMath(char *str);
		DynamicMath(std::string str);
		~DynamicMath();
};
