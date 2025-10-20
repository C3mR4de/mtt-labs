#ifndef MT19937_H
#define MT19937_H

#include <stddef.h>

#define MT_N 624

typedef struct
{
    unsigned long mt[MT_N];
    size_t        index;
}
MT19937;

void          MT19937_Init(MT19937* gen, unsigned long seed);
unsigned long MT19937_Rand(MT19937* gen);
double        MT19937_RandRange(MT19937* gen, double a, double b);

#endif
