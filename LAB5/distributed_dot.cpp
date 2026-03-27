#include <mpi.h>
#include <iostream>
#include <vector>

#define TOTAL_N 500000000   // 500 million

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double multiplier;

    // ---------------- STEP 1: INPUT + BROADCAST ----------------
    if (rank == 0) {
        std::cout << "Enter scaling multiplier: ";
        std::cin >> multiplier;
    }

    // Send multiplier to all processes
    MPI_Bcast(&multiplier, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // ---------------- STEP 2: LOCAL SIZE ----------------
    long long local_n = TOTAL_N / size;

    // Allocate only local chunks
    std::vector<double> A(local_n);
    std::vector<double> B(local_n);

    // ---------------- STEP 3: LOCAL INITIALIZATION ----------------
    for (long long i = 0; i < local_n; i++) {
        A[i] = 1.0;
        B[i] = 2.0 * multiplier;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    // ---------------- STEP 4: LOCAL DOT PRODUCT ----------------
    double local_sum = 0.0;

    for (long long i = 0; i < local_n; i++) {
        local_sum += A[i] * B[i];
    }

    // ---------------- STEP 5: GLOBAL REDUCTION ----------------
    double global_sum = 0.0;

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE,
               MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    // ---------------- OUTPUT ----------------
    if (rank == 0) {
        std::cout << "Final Dot Product = " << global_sum << std::endl;
        std::cout << "Execution Time: " << (end - start) << " seconds" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
