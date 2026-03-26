#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int N = 10;
    int numbers[N];

    srand(time(0) + rank);

    for (int i = 0; i < N; i++)
        numbers[i] = rand() % 1001;

    int local_max = numbers[0], local_min = numbers[0];

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    for (int i = 1; i < N; i++) {
        if (numbers[i] > local_max) local_max = numbers[i];
        if (numbers[i] < local_min) local_min = numbers[i];
    }

    int global_max, global_min;

    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    struct {
        int value;
        int rank;
    } local_maxloc, global_maxloc, local_minloc, global_minloc;

    local_maxloc.value = local_max;
    local_maxloc.rank = rank;

    local_minloc.value = local_min;
    local_minloc.rank = rank;

    MPI_Reduce(&local_maxloc, &global_maxloc, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_minloc, &global_minloc, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (rank == 0) {
        std::cout << "Global Max = " << global_max 
                  << " (Process " << global_maxloc.rank << ")\n";
        std::cout << "Global Min = " << global_min 
                  << " (Process " << global_minloc.rank << ")\n";
        std::cout << "Execution Time: " << (end - start) << " seconds\n";
    }

    MPI_Finalize();
    return 0;
}
