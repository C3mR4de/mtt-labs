#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef NDEBUG
    #define LOG(...)
#else
    #define LOG(msg) printf("%s\n", msg);
#endif

#endif
