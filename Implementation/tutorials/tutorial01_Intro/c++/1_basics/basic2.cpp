// #include <iostream>
#include <cstdlib>
#include <stdio.h> // same as #include <cstdio>
// https://www.cplusplus.com/reference/cstdlib/
// https://www.cplusplus.com/reference/cstdio/

int main(int argc, char** argv)
{
// argc: the argument count: strings parsed to the executable plus the executable.
//  i.e. here, we have argc=2 (./factorial N) = 2 arguments
// argv: the argument vector : returns the arguments in the order parsed in terminal
//  i.e. here, we have argv[0]=./factorial and argv[1]=<string parsed, N>
    if(argc<2){
      printf("Usage: %s N \n", argv[0]);
      printf("Prints the factorial of N and all the intermediate.\n");
      return 0;
    }
    const int N = atoi(argv[1]);

    // print the header
    printf("Calculating the factorial: %d!\n",N);
    // std::cout << "n" << std::setw(15) << "n!" << std::endl;

    int product = 1;
    for(int n = 0; n <= N; ++n)
    {
        if (n != 0)
        {
            // calculate the next next factorial
            product *= n;
        }

        // output the factorial
        printf("%3d  %15d\n",n,product);
    // string formatting : https://www.cplusplus.com/reference/cstdio/printf/
    }

    // tell the caller program that no error has occurred
    return 0;
}
