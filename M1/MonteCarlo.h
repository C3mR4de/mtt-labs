#ifndef MONTECARLO_H
#define MONTECARLO_H

#define mcResult(T) struct { T elapsed_time, area; }
typedef mcResult(double) mcResultd;

mcResultd MonteCarlo_Calculate(unsigned long radius, unsigned long threads_count, unsigned long tries, unsigned long seed);

#endif
