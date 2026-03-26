#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include<omp.h>

void correlate(int ny, int nx, const float* data, float* result);

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "Usage: ./program ny nx\n";
        return 1;
    }

    int ny = atoi(argv[1]);
    int nx = atoi(argv[2]);

    std::vector<float> data(ny * nx);
    std::vector<float> result(ny * ny);

    // Fill with random data
    srand(time(0));
    for (int i = 0; i < ny * nx; i++) {
        data[i] = rand() % 100;
    }
    
    double start = omp_get_wtime();
    
    correlate(ny, nx, data.data(), result.data());
    
    double end = omp_get_wtime();
    
    std::cout << "\nExecution Time: " << (end - start) << " seconds\n";

    //std::cout << "Correlation matrix (lower triangle):\n";

    //for (int i = 0; i < ny; i++) {
    //    for (int j = 0; j <= i; j++) {
    //        std::cout << result[i + j * ny] << " ";
    //    }
    //    std::cout << "\n";
    //}

    return 0;
}
