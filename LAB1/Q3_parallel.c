#include <stdio.h>
#include <omp.h>

int main() {
    long n = 100000000;
    double step = 1.0 / n;
    double sum = 0.0;

    double start = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum)
    for(long i=0;i<n;i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x*x);
    }

    double pi = step * sum;
    double end = omp_get_wtime();

    printf("Pi = %.10f\n", pi);
    printf("Time = %f\n", end - start);
}

