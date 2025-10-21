#ifndef MT19937_H
#define MT19937_H

#include <stdint.h>
#include <stddef.h>

#define MT_N 624

typedef struct
{
    uint32_t mt[MT_N];
    size_t   index;
}
MT19937;

void     MT19937_Init(MT19937* gen, uint32_t seed);
uint32_t MT19937_Rand(MT19937* gen);
double   MT19937_RandRange(MT19937* gen, double a, double b);

#endif
