unsigned int factorial(const unsigned int n)
{
    unsigned int fac = 1;

    for (unsigned int i = 2; i <= n; ++i)
    {
        fac *= i;
    }

    return fac;
}
