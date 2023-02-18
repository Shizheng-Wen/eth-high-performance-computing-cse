#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>

// POST: returns n!
unsigned int factorial(int n)
{
    unsigned int fac = 1;

    for (int i = 2; i <= n; ++i)
    {
        fac *= i;
    }

    return fac;
}

int main(int argc, char** argv)
{
    if(argc<2){
      printf("Usage: %s N \n", argv[0]);
      printf("Prints the factorial of N and all the intermediate.\n");
      return 0;
    }
    const int N = atoi(argv[1]);
    // print the header
    printf("Calculating the factorial: %d!\n",N);

    for(int n = 0; n <= N; ++n)
    {
        // output the factorial
        std::cout << n << std::setw(12) << factorial(n) << std::endl;
    }

    // indicate that no error has occurred
    return 0;
}
