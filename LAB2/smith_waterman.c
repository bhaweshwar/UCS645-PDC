#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

// Scoring scheme
#define MATCH 2
#define MISMATCH -1
#define GAP -1

// Maximum function
int max(int a, int b, int c, int d) {
    int max = a;
    if (b > max) max = b;
    if (c > max) max = c;
    if (d > max) max = d;
    return max;
}

int main() {

    // Example DNA sequences (you can modify these)
    char seq1[] = "ACACACTA";
    char seq2[] = "AGCACACA";

    int m = strlen(seq1);
    int n = strlen(seq2);

    // Allocate scoring matrix (size (m+1) x (n+1))
    int H[m+1][n+1];

    // Initialize matrix with zeros
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            H[i][j] = 0;
        }
    }

    int max_score = 0;

    double start = omp_get_wtime();

    // -------------------------------
    // Wavefront Parallelization
    // -------------------------------
    // Traverse diagonals (anti-diagonals)
    for (int k = 2; k <= m + n; k++) {

        // Parallelize along diagonal
        #pragma omp parallel for reduction(max:max_score) schedule(runtime)
        for (int i = 1; i <= m; i++) {

            int j = k - i;

            // Check valid indices
            if (j >= 1 && j <= n) {

                // Match or mismatch score
                int score_diag = H[i-1][j-1] + 
                    (seq1[i-1] == seq2[j-1] ? MATCH : MISMATCH);

                // Gap scores
                int score_up = H[i-1][j] + GAP;
                int score_left = H[i][j-1] + GAP;

                // Smith-Waterman recurrence
                H[i][j] = max(0, score_diag, score_up, score_left);

                // Track maximum score
                if (H[i][j] > max_score) {
                    max_score = H[i][j];
                }
            }
        }
    }

    double end = omp_get_wtime();

    // Print scoring matrix (for understanding)
    printf("Scoring Matrix:\n");
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            printf("%3d ", H[i][j]);
        }
        printf("\n");
    }

    printf("\nMaximum Alignment Score: %d\n", max_score);
    printf("Execution Time: %f seconds\n", end - start);

    return 0;
}
