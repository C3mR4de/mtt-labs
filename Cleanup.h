#ifndef CLEANUP_H
#define CLEANUP_H

#include <stdio.h>

#define CLEANUP_IF(cond, msg) do { if (cond) { fprintf(stderr, "%s\n", msg); goto cleanup; } } while (0)

#endif
