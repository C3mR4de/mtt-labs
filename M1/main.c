#include <stdlib.h>
#include <inttypes.h>
#include "MonteCarlo.h"
#include "Cleanup.h"

int main(const int argc, const char* const argv[])
{
    int exit_code = EXIT_FAILURE;
    CLEANUP_IF(argc == 1 || argc > 3, "Wrong args count!");

    char* endptr;
    const long tries = strtol(argv[1], &endptr, 10);
    CLEANUP_IF(*endptr != '\0' || tries <= 0, "Wrong tries count!");

    long seed = 0;

    if (argc == 3)
    {
        seed = strtol(argv[2], &endptr, 10);
        CLEANUP_IF(*endptr != '\0' || seed < 0, "Wrong seed!");
    }

    int64_t radius, threads_count;

    while (scanf("%" SCNd64 "%" SCNd64, &radius, &threads_count) == 2)
    {
        CLEANUP_IF(radius <= 0 || threads_count <= 0, "Wrong input data!");

        mcResultd result = MonteCarlo_Calculate(radius, threads_count, tries, seed);
        CLEANUP_IF(result.area == 0, "Error while processing!");

        printf("%.3f %.3f\n", result.elapsed_time, result.area);
    }

    exit_code = EXIT_SUCCESS;

cleanup:

    return exit_code;
}
