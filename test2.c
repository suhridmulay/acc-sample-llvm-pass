#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "[ERROR] Insufficient number of arguments\n");
        fprintf(stderr, "[INFO] Usage: ./<pi-estimator> <iterations>\n");
        exit(-1);
    }

    size_t ITERS = atoi(argv[1]);
    float pi = 3;
    size_t inside = 0;
    size_t total = ITERS;
    srand(time(NULL));

    for (size_t i = 0; i < ITERS; i++) {
        double x = ((float) rand() / (float) RAND_MAX);
        double y = ((float) rand() / (float) RAND_MAX);
        if (x * x + y * y < 1) {
            inside += 1;
        }
    }

    fprintf(stdout, "My estimate of pi is: %f\n", ((float) inside / (float) total) * 4);
}