#ifndef MT19937_H
#define MT19937_H

#include <stdint.h>

void MT19937_Srand(unsigned long seed);
unsigned long MT19937_Rand(void);
double MT19937_RandRange(double a, double b);

#endif
