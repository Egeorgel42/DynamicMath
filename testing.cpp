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
	printTest(-1);
	DynamicMath test1 = 1;
	DynamicMath test2 = "60.4";
	DynamicMath test3 = "0.6";
	printTest(test2 + test3);
}