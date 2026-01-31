# PDC Assignment – OpenMP Programs

This repository contains implementations of basic parallel programs using OpenMP.

## Q1: DAXPY Loop
- File: daxpy_parallel.c  
Operation: X[i] = a * X[i] + Y[i]

Compile:
gcc -fopenmp daxpy_parallel.c -o daxpy_parallel

Run:
./daxpy_parallel

---

## Q2: Matrix Multiplication

### Serial Version
- File: matrix_serial.c

Compile:
gcc -fopenmp matrix_serial.c -o matrix_serial

Run:
./matrix_serial

### Parallel 1D Version
- File: matrix_1D.c

Compile:
gcc -fopenmp matrix_1D.c -o matrix_1D

Run:
./matrix_1D

### Parallel 2D Version
- File: matrix_2D.c

Compile:
gcc -fopenmp matrix_2D.c -o matrix_2D

Run:
./matrix_2D

---

## Q3: Calculation of Pi

### Serial Version
- File: Q3_serial.c

Compile:
gcc -fopenmp Q3_serial.c -o Q3_serial

Run:
./Q3_serial

### Parallel Version
- File: Q3_parallel.c

Compile:
gcc -fopenmp Q3_parallel.c -o Q3_parallel

Run:
./Q3_parallel

---

To control number of threads:
export OMP_NUM_THREADS=4
# UCS645-PDC
