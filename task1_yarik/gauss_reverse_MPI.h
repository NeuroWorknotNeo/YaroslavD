#ifndef GAUSS_REVERSE_MPI_H
#define GAUSS_REVERSE_MPI_H

#include <stdlib.h>
#include <math.h>
#include <mpi.h>

/* Для поиска максимума вместе с его номером (MPI_DOUBLE_INT + MPI_MAXLOC). */
typedef struct {
	double value;
	int index;
} MAX;

/* Обратная матрица методом Гаусса с выбором главного элемента по всей матрице.
   n        - размер матрицы,
   local_a  - строки матрицы A этого процесса (портятся),
   local_x  - сюда кладутся строки обратной матрицы,
   buffer   - рабочий массив из 2n чисел,
   col      - рабочий массив из n целых.
   Возвращает 0, если всё получилось, и -1, если матрица вырождена. */
int gauss_reverse_mpi(int n, double *local_a, double *local_x, int rank, int size,
                      double *buffer, int *col);

/* Невязки. Дополнительной памяти не выделяют, работают через buffer (2n чисел). */
void r_mpi(int n, double *local_a, double *local_b, double *r, int rank, int size, double *buffer);
void r1_r2_mpi(int n, double *local_a, double *local_x, double *r1, double *r2,
               int rank, int size, double *buffer);

#endif
