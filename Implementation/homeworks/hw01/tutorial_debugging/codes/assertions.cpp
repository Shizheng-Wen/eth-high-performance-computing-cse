#include <cassert>
#include <iostream>
#include <cmath>

int assert1(){
	int a = 4;
	int b;
	std::cout << "give in an integer number b > 4: " << std::flush;
	std::cin >> b;
	std::cout << std::endl;
	assert(a<b);
	return 0;
}

int assert2(){
	double a = 2;
	double b;
	std::cout << "give in a number b /= 0: " << std::flush;
	std::cin >> b;
	std::cout << std::endl;
	double c = a/b;
	assert(!std::isnan(c));
	assert(!std::isinf(c));
	return 0;
}

int assert3(){
	assert(false && "This will always fail.\n");
	return 0;
}

int main(){
	assert1();

	assert2();

	assert3();

	return 0;
}
