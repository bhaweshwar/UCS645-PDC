#include <cmath>
#include <vector>
#include <omp.h>
#include <iostream>

// Toggle this: 0 = sequential, 1 = parallel
#define USE_OPENMP 1

void correlate(int ny, int nx, const float* data, float* result)
{

    std::vector<double> mean(ny, 0.0);
    std::vector<double> stddev(ny, 0.0);

    // Step 1: Compute mean
    for (int i = 0; i < ny; i++) {
        for (int x = 0; x < nx; x++) {
            mean[i] += data[x + i * nx];
        }
        mean[i] /= nx;
    }

    // Step 2: Compute standard deviation
    for (int i = 0; i < ny; i++) {
        for (int x = 0; x < nx; x++) {
            double val = data[x + i * nx] - mean[i];
            stddev[i] += val * val;
        }
        stddev[i] = sqrt(stddev[i]);
    }

    // Step 3: Correlation
    #if USE_OPENMP
    #pragma omp parallel for schedule(static)
    #endif
    for (int i = 0; i < ny; i++) {
        for (int j = 0; j <= i; j++) {

            double sum = 0.0;

            #pragma omp simd reduction(+:sum)
            for (int x = 0; x < nx; x++) {
                double a = data[x + i * nx] - mean[i];
                double b = data[x + j * nx] - mean[j];
                sum += a * b;
            }

            double denom = stddev[i] * stddev[j];

            if (denom == 0)
                result[i + j * ny] = 0;
            else
                result[i + j * ny] = sum / denom;
        }
    }
}
