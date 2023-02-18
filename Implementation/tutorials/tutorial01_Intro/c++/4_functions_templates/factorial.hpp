#ifndef FACTORIAL_HPP
#define FACTORIAL_HPP

// POST: returns n!
template <typename T>
T factorial(const T n)
{
    T fac = 1;

    for (T i = 2; i <= n; ++i)
    {
        fac *= i;
    }

    return fac;
}

#endif // FACTORIAL_HPP
