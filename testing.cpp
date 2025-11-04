#include "DynamicMath.hpp"

template <typename T, typename std::enable_if<!std::is_integral<T>::value>::type* = nullptr>
void 	printTest(T var)
{
	std::cout << "var: " << var << std::endl;
	DynamicMath test = var;
	test.printData();
}

template <typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
void 	printTest(T var)
{
	std::cout << "var: " << std::to_string(var) << std::endl;
	DynamicMath test = var;
	test.printData();
}

void testOperation(DynamicMath &nbr1, std::string operation, std::regex_token_iterator<std::string::iterator> &it)
{
	std::regex_token_iterator<std::string::iterator> end;
	it++;
	if (it == end)
		throw;
	DynamicMath nbr2(*it);
	if (operation == "+")
		nbr1 += nbr2;
	else if (operation == "-")
		nbr1 -= nbr2;
	else if (operation == "*")
		nbr1 *= nbr2;
	else if (operation == "%")
		nbr1 %= nbr2;
	else
		throw;
}

void 	printTestChar(std::string str)
{
	std::regex split = std::regex(R"([^\s]+)");
	std::regex_token_iterator<std::string::iterator> it(str.begin(), str.end(), split);
	std::regex_token_iterator<std::string::iterator> end;
	if (it == end)
		throw;
	DynamicMath nbr1(*it);
	it++;
	for (; it != end; it++)
	{
		testOperation(nbr1, *it, it);
		nbr1.printData();
	}
}

int main(int argc, char **argv)
{
	if (argc == 2)
		printTestChar(argv[1]);
}