#include "MonteCarlo.h"
#include <stdlib.h>
#include <assert.h>
#include <threads.h>
#include "Cleanup.h"
#include "MT19937.h"

typedef struct
{
    unsigned long radius, tries, seed;
    size_t result;
}
mcParams;

static int __MonteCarlo_Generate(void* const args)
{
    mcParams* const params = (mcParams*)args;

    MT19937_Srand(params->seed);

    size_t count = 0;

    for (unsigned long i = 0; i < params->tries; ++i)
    {
        const double x = MT19937_RandRange(0, params->radius);
        const double y = MT19937_RandRange(0, params->radius);

        count += (x * x + y * y <= (double)params->radius * params->radius);
    }

    params->result = count;
    return EXIT_SUCCESS;
}

mcResultd MonteCarlo_Calculate(const unsigned long radius, const unsigned long threads_count, const unsigned long tries, const unsigned long seed)
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

    MT19937_Srand(seed);

    for (size_t i = 0; i < threads_count - 1; ++i)
    {
        results[i] = (mcParams){radius, tries / threads_count, MT19937_Rand(), 0};
        CLEANUP_IF(thrd_create(threads + i, __MonteCarlo_Generate, results + i) != thrd_success, "Error creating thread!");
        ++created_threads_count;
    }

    results[threads_count - 1] = (mcParams){radius, tries - (tries / threads_count) * (threads_count - 1), MT19937_Rand(), 0};
    __MonteCarlo_Generate(results + threads_count - 1);

cleanup:

    for (size_t i = 0; i < created_threads_count; ++i)
        thrd_join(threads[i], NULL);

    if (created_threads_count == threads_count - 1)
    {
        size_t total_count = 0;

        for (size_t i = 0; i < threads_count; ++i)
            total_count += results[i].result;

        result = (mcResultd){4 * (double)total_count / tries, 0};
    }

    free(results);
    free(threads);

    return result;
}
