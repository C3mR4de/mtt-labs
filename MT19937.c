#include "MT19937.h"
#include <threads.h>
#include <stdint.h>

#define MT_M          397
#define MT_MATRIX_A   0x9908B0DFUL
#define MT_UPPER_MASK 0x80000000UL
#define MT_LOWER_MASK 0x7FFFFFFFUL

void MT19937_Init(MT19937* const gen, const unsigned long seed)
{
    gen->mt[0] = seed;

    for (gen->index = 1; gen->index < MT_N; ++gen->index)
    {
        gen->mt[gen->index] = (1812433253UL * (gen->mt[gen->index - 1] ^ (gen->mt[gen->index - 1] >> 30)) + gen->index);
    }

    gen->index = MT_N;
}

static void __MT19937_Generate(MT19937* const gen)
{
    unsigned long y;
    static const unsigned long mag01[2] = {0x0UL, MT_MATRIX_A};
    int kk = 0;

    for (; kk < MT_N - MT_M; ++kk)
    {
        y = (gen->mt[kk] & MT_UPPER_MASK) | (gen->mt[kk + 1] & MT_LOWER_MASK);
        gen->mt[kk] = gen->mt[kk + MT_M] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }

    for (; kk < MT_N - 1; ++kk)
    {
        y = (gen->mt[kk] & MT_UPPER_MASK) | (gen->mt[kk + 1] & MT_LOWER_MASK);
        gen->mt[kk] = gen->mt[kk + (MT_M - MT_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }

    y = (gen->mt[MT_N - 1] & MT_UPPER_MASK) | (gen->mt[0] & MT_LOWER_MASK);
    gen->mt[MT_N - 1] = gen->mt[MT_M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

    gen->index = 0;
}

unsigned long MT19937_Rand(MT19937* const gen)
{
    if (gen->index >= MT_N)
        __MT19937_Generate(gen);

    unsigned long y = gen->mt[gen->index++];

    y ^= (y >> 11);
    y ^= (y << 7)  & 0x9D2C5680UL;
    y ^= (y << 15) & 0xEFC60000UL;
    y ^= (y >> 18);

    return y;
}

double MT19937_RandRange(MT19937* const gen, const double a, const double b)
{
    if (a >= b)
        return a;

    double r = MT19937_Rand(gen) / 4294967296.0;
    return a + r * (b - a);
}
