#include <iostream>
#include <iomanip>
#include "max.hpp"

int main()
{
	int i1, i2;
	float f1, f2;
	char c1, c2;


  i1 = 9; i2 = 12;
	std::cout << myops::max(i1, i2) <<" is larger." << std::endl;

  f1 = 123.121; f2 = 431.9;
	std::cout << myops::max(f1, f2) <<" is larger." << std::endl;

  c1 = 'a'; c2 = '4';
	std::cout << myops::max(c1, c2) << " has larger ASCII value." << std::endl;

	return 0;
}
