#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <stdint.h>

#define mcResult(T) struct { T elapsed_time, area; }
typedef mcResult(double) mcResultd;

mcResultd MonteCarlo_Calculate(uint32_t radius, uint32_t threads_count, uint32_t tries, uint32_t seed);

#endif
