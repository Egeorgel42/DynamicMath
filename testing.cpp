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

int main(int argc, char **argv)
{
	if (argc == 2)
		printTest(argv[1]);
	DynamicMath test1 = "-9.000000000000000001";
	DynamicMath test3 = -9000000000000000000;
	printTest(test3 + test1 + test1 + test1);
}