#include <iostream>

int main(int argc)
{
	int a = 34, b;
	
	b = (a > 21) ? a : -a;

	std::cout << b << std::endl;

	return 0;
}