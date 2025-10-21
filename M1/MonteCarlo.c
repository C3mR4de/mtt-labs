#include "MonteCarlo.h"
#include <stdlib.h>
#include <assert.h>
#include <threads.h>
#include <time.h>
#include "Cleanup.h"
#include "MT19937.h"

typedef struct
{
    uint32_t radius, tries, seed;
    size_t   result;
}
mcParams;

static int __MonteCarlo_Generate(void* const args)
{
    mcParams* const params = (mcParams*)args;

    MT19937 generator;
    MT19937_Init(&generator, params->seed);

    size_t count = 0;

    for (uint32_t i = 0; i < params->tries; ++i)
    {
        const double x = MT19937_RandRange(&generator, 0, params->radius);
        const double y = MT19937_RandRange(&generator, 0, params->radius);

        count += (x * x + y * y <= (double)params->radius * params->radius);
    }

    params->result = count;
    return EXIT_SUCCESS;
}

mcResultd MonteCarlo_Calculate(const uint32_t radius, const uint32_t threads_count, const uint32_t tries, const uint32_t seed)
{
    assert(radius > 0 && threads_count > 0);
    mcResultd result = {0, 0};

    thrd_t*   threads = NULL;
    mcParams* results = NULL;

    threads = malloc((threads_count - 1) * sizeof(thrd_t));
    CLEANUP_IF(!threads, "Error allocating memory for threads!");

    results = malloc(threads_count * sizeof(mcParams));
    CLEANUP_IF(!results, "Error allocating memory for results!");

    size_t created_threads_count = 0;

    MT19937 generator;
    MT19937_Init(&generator, seed);

    struct timespec start_time, end_time;
    timespec_get(&start_time, TIME_UTC);

    for (size_t i = 0; i < threads_count - 1; ++i)
    {
        results[i] = (mcParams)
        {
            .radius = radius,
            .tries  = tries / threads_count,
            .seed   = MT19937_Rand(&generator)
        };

        CLEANUP_IF(thrd_create(threads + i, __MonteCarlo_Generate, results + i) != thrd_success, "Error creating thread!");
        ++created_threads_count;
    }

    results[threads_count - 1] = (mcParams)
    {
        .radius = radius,
        .tries  = tries - (tries / threads_count) * (threads_count - 1),
        .seed   = MT19937_Rand(&generator)
    };

    __MonteCarlo_Generate(results + threads_count - 1);

cleanup:

    for (size_t i = 0; i < created_threads_count; ++i)
        thrd_join(threads[i], NULL);

    if (created_threads_count == threads_count - 1)
    {
        timespec_get(&end_time, TIME_UTC);
        const double elapsed_ms = (end_time.tv_sec  - start_time.tv_sec) * 1000.0
                                + (end_time.tv_nsec - start_time.tv_nsec) / 1.0e6;

        size_t total_count = 0;

        for (size_t i = 0; i < threads_count; ++i)
            total_count += results[i].result;

        result = (mcResultd){elapsed_ms, radius * radius * 4.0 * total_count / tries};
    }

    free(results);
    free(threads);

    return result;
}
