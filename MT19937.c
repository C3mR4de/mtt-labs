#include "MT19937.h"
#include <threads.h>

#define MT_N          624
#define MT_M          397
#define MT_MATRIX_A   0x9908B0DFUL
#define MT_UPPER_MASK 0x80000000UL
#define MT_LOWER_MASK 0x7FFFFFFFUL

thread_local static uint32_t mt[MT_N];
thread_local static size_t   mt_index = MT_N + 1;

void MT19937_Srand(const unsigned long seed)
{
    mt[0] = seed;

    for (mt_index = 1; mt_index < MT_N; ++mt_index)
    {
        mt[mt_index] = (1812433253UL * (mt[mt_index - 1] ^ (mt[mt_index - 1] >> 30)) + mt_index);
    }
}

static void __MT19937_Generate(void)
{
    unsigned long y;
    int kk;
    static const unsigned long mag01[2] = {0x0UL, MT_MATRIX_A};

    for (kk = 0; kk < MT_N - MT_M; ++kk)
    {
        y = (mt[kk] & MT_UPPER_MASK) | (mt[kk + 1] & MT_LOWER_MASK);
        mt[kk] = mt[kk + MT_M] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }

    for (; kk < MT_N - 1; ++kk)
    {
        y = (mt[kk] & MT_UPPER_MASK) | (mt[kk + 1] & MT_LOWER_MASK);
        mt[kk] = mt[kk + (MT_M - MT_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }

    y = (mt[MT_N - 1] & MT_UPPER_MASK) | (mt[0] & MT_LOWER_MASK);
    mt[MT_N - 1] = mt[MT_M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

    mt_index = 0;
}

unsigned long MT19937_Rand(void)
{
    unsigned long y;

    if (mt_index >= MT_N)
    {
        if (mt_index == MT_N + 1) MT19937_Srand(5489UL);
        __MT19937_Generate();
    }

    y = mt[mt_index++];

    y ^= (y >> 11);
    y ^= (y << 7)  & 0x9D2C5680UL;
    y ^= (y << 15) & 0xEFC60000UL;
    y ^= (y >> 18);

    return y;
}

double MT19937_RandRange(const double a, const double b)
{
    if (a >= b) return a;
    double r = MT19937_Rand() * (1.0 / (double)((unsigned long)~0)); // [0,1)
    return a + r * (b - a);
}
