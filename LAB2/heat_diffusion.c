#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100            // Grid size (NxN plate)
#define STEPS 500        // Number of time steps
#define ALPHA 0.1        // Diffusion constant

int main() {

    // Temperature grids
    double current[N][N];     // Current time step
    double next[N][N];        // Next time step

    // Initialize grid
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            current[i][j] = 0.0;

            // Make center hot (heat source)
            if (i > N/4 && i < 3*N/4 && j > N/4 && j < 3*N/4) {
                current[i][j] = 100.0;
            }
        }
    }

    double start = omp_get_wtime();

    // Time loop
    for (int t = 0; t < STEPS; t++) {

        double total_heat = 0.0;

        // Parallelize spatial loops
        #pragma omp parallel for reduction(+:total_heat) schedule(runtime)
        for (int i = 1; i < N-1; i++) {
            for (int j = 1; j < N-1; j++) {

                // Finite difference update (average of neighbors)
                next[i][j] = current[i][j] + ALPHA * (
                    current[i+1][j] +
                    current[i-1][j] +
                    current[i][j+1] +
                    current[i][j-1] -
                    4 * current[i][j]
                );

                // Accumulate total heat (for analysis)
                total_heat += next[i][j];
            }
        }

        // Copy next -> current for next iteration
        #pragma omp parallel for schedule(runtime)
        for (int i = 1; i < N-1; i++) {
            for (int j = 1; j < N-1; j++) {
                current[i][j] = next[i][j];
            }
        }

        // (Optional) Print heat at some steps
        if (t % 100 == 0) {
            printf("Step %d, Total Heat: %f\n", t, total_heat);
        }
    }

    double end = omp_get_wtime();

    printf("\nSimulation Completed.\n");
    printf("Execution Time: %f seconds\n", end - start);

    return 0;
}
