#ifndef OUTPUT_MPI_H
#define OUTPUT_MPI_H

#include <stdio.h>
#include <mpi.h>

/* Печатает на экран не более r строк и r столбцов матрицы l x n,
   распределённой по строкам циклически. buffer - не менее n чисел. */
void output_mpi(int r, int l, int n, double *local_a, int rank, int size, double *buffer);

#endif
