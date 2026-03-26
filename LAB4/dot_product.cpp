#include <mpi.h>
#include <iostream>

#define N 8

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int A[N], B[N];
    int local_size = N / size;

    int local_A[local_size], local_B[local_size];
    int local_dot = 0, global_dot = 0;

    if (rank == 0) {
        int tempA[N] = {1,2,3,4,5,6,7,8};
        int tempB[N] = {8,7,6,5,4,3,2,1};

        for (int i = 0; i < N; i++) {
            A[i] = tempA[i];
            B[i] = tempB[i];
        }
    }

    MPI_Scatter(A, local_size, MPI_INT, local_A, local_size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, local_size, MPI_INT, local_B, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    for (int i = 0; i < local_size; i++)
        local_dot += local_A[i] * local_B[i];

    MPI_Reduce(&local_dot, &global_dot, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (rank == 0) {
        std::cout << "Dot Product = " << global_dot << std::endl;
        std::cout << "Execution Time: " << (end - start) << " seconds\n";
    }

    MPI_Finalize();
    return 0;
}
