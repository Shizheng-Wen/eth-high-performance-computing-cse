#include <iostream>
#include <iomanip>
// https://www.cplusplus.com/reference/cstdlib/
// https://www.cplusplus.com/reference/cstdio/
// string formatting : https://www.cplusplus.com/reference/cstdio/printf/
// https://www.cplusplus.com/reference/iostream/
// https://www.cplusplus.com/reference/iomanip/


// in case you want to get arguments passed to your program
// (such as "./basic a b c"), use the following definition:
//     int main(int argc, char** argv) { ... }
int main()
{
    const int N = 9;
    // print the header
    std::cout << "Calculating the factorial for n = 0, ..., N:" << std::endl;
    std::cout << "n" << std::setw(12) << "n!" << std::endl;

    int product = 1;
    for(int n = 0; n <= N; ++n)
    {
        if (n != 0)
        {
            // calculate the next next factorial
            product *= n;
        }
        
        // output the factorial
        std::cout << n << std::setw(12) << product << std::endl;
    }

    // tell the caller program that no error has occurred
    return 0;
}
