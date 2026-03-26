#include <mpi.h>
#include <iostream>

#define N 100

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data[N];
    int local_size = N / size;
    int local_data[local_size];

    int local_sum = 0, global_sum = 0;

    if (rank == 0) {
        for (int i = 0; i < N; i++)
            data[i] = i + 1;
    }

    MPI_Scatter(data, local_size, MPI_INT,
                local_data, local_size, MPI_INT,
                0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    for (int i = 0; i < local_size; i++)
        local_sum += local_data[i];

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT,
               MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (rank == 0) {
        std::cout << "Final Sum = " << global_sum << std::endl;
        std::cout << "Average = " << (double)global_sum / N << std::endl;
        std::cout << "Execution Time: " << (end - start) << " seconds\n";
    }

    MPI_Finalize();
    return 0;
}
