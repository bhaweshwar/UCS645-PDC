#include <mpi.h>
#include <iostream>

// Function to check perfect number
bool isPerfect(int n) {
    if (n < 2) return false;

    int sum = 1; // 1 is always a divisor

    for (int i = 2; i <= n / 2; i++) {
        if (n % i == 0)
            sum += i;
    }

    return (sum == n);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int max_value;

    // Master takes input
    if (rank == 0) {
        std::cout << "Enter maximum value: ";
        std::cin >> max_value;
    }

    // Broadcast max value
    MPI_Bcast(&max_value, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    // ================= MASTER =================
    if (rank == 0) {
        int current = 2;
        int active_workers = size - 1;

        while (active_workers > 0) {
            int msg;
            MPI_Status status;

            // Receive from any worker
            MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0,
                     MPI_COMM_WORLD, &status);

            int worker = status.MPI_SOURCE;

            // If positive → perfect number
            if (msg > 0) {
                std::cout << "Perfect Number: " << msg << std::endl;
            }

            // Send next number if available
            if (current <= max_value) {
                MPI_Send(&current, 1, MPI_INT, worker, 0, MPI_COMM_WORLD);
                current++;
            } else {
                // Send termination signal
                int stop = 0;
                MPI_Send(&stop, 1, MPI_INT, worker, 0, MPI_COMM_WORLD);
                active_workers--;
            }
        }
    }

    // ================= WORKERS =================
    else {
        int number;

        // Initial request (0 means starting)
        int request = 0;
        MPI_Send(&request, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        while (true) {
            // Receive number from master
            MPI_Recv(&number, 1, MPI_INT, 0, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (number == 0) {
                // Stop signal
                break;
            }

            int result;

            if (isPerfect(number)) {
                result = number;       // positive → perfect
            } else {
                result = -number;      // negative → not perfect
            }

            // Send result back to master
            MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (rank == 0) {
        std::cout << "Execution Time: " << (end - start) << " seconds\n";
    }

    MPI_Finalize();
    return 0;
}
