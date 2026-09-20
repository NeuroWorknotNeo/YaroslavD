#ifndef INPUT_MPI_H
#define INPUT_MPI_H

#include <stdio.h>
#include <mpi.h>

/* Матрица распределена по процессам по строкам циклически:
   глобальная строка g лежит на процессе g%size в локальной строке g/size. */

/* s == 0 - читаем матрицу из файла, иначе строим по формуле номер s. */
int input_mpi(int s, char *filename, int n, double *local_a, double *buffer, int rank, int size);

int finput_mpi(char *filename, int n, double *local_a, double *buffer, int rank, int size);
int sinput_mpi(int s, int n, double *local_a, int rank, int size);
double formula_mpi(int s, int n, int i, int j);

#endif
