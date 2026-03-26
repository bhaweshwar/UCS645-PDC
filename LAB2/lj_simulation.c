#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N 500              // Number of particles
#define DIM 3              // 3D space
#define EPSILON 1.0        // Lennard-Jones parameter
#define SIGMA 1.0          // Lennard-Jones parameter

// Structure to store particle positions
typedef struct {
    double x[DIM];         // x, y, z coordinates
} Particle;

int main() {

    Particle particles[N];                 // Array of particles
    double force[N][DIM];                  // Force on each particle
    double total_energy = 0.0;             // Total potential energy

    // Initialize particles with random positions
    for (int i = 0; i < N; i++) {
        for (int d = 0; d < DIM; d++) {
            particles[i].x[d] = (double)rand() / RAND_MAX;
            force[i][d] = 0.0;             // Initialize force to zero
        }
    }

    double start_time = omp_get_wtime();   // Start timer

    // Parallel region
    #pragma omp parallel
    {
        double local_energy = 0.0;  // Each thread keeps its own energy

        // Parallelize outer loop
        #pragma omp for schedule(runtime)
        for (int i = 0; i < N; i++) {

            for (int j = i + 1; j < N; j++) {

                double dx[DIM];
                double r2 = 0.0;

                // Compute distance vector and squared distance
                for (int d = 0; d < DIM; d++) {
                    dx[d] = particles[i].x[d] - particles[j].x[d];
                    r2 += dx[d] * dx[d];
                }

                double r = sqrt(r2);

                // --- NO CUTOFF: compute for ALL particle pairs ---

                double sr = SIGMA / r;
                double sr6 = pow(sr, 6);
                double sr12 = sr6 * sr6;

                // Lennard-Jones potential energy
                double energy = 4 * EPSILON * (sr12 - sr6);
                local_energy += energy;

                // Force magnitude
                double force_mag = 24 * EPSILON * (2 * sr12 - sr6) / r2;

                // Update forces (race condition handled using atomic)
                for (int d = 0; d < DIM; d++) {

                    double f = force_mag * dx[d];

                    #pragma omp atomic
                    force[i][d] += f;

                    #pragma omp atomic
                    force[j][d] -= f;
                }
            }
        }

        // Combine thread-local energy into global total energy
        #pragma omp atomic
        total_energy += local_energy;
    }

    double end_time = omp_get_wtime();     // End timer

    // Print results
    printf("Total Potential Energy: %f\n", total_energy);
    printf("Execution Time: %f seconds\n", end_time - start_time);

    return 0;
}
