#include <stdio.h>
#include <omp.h>

#define N 500

int main() {
    static double A[N][N], B[N][N], C[N][N];

    // Initialize
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = 1.0;
            B[i][j] = 2.0;
            C[i][j] = 0.0;
        }
    }

    double start = omp_get_wtime();

    // Parallel 1D (rows)
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    double end = omp_get_wtime();

    printf("Parallel 1D Time: %f seconds\n", end - start);
    printf("C[0][0] = %f\n", C[0][0]);

    return 0;
}

