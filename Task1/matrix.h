#ifndef MATRIX_H
#define MATRIX_H

/* Заполнить матрицу a (n x n) по формуле номер k из задания. */
void fill_matrix(double *a, int n, int k);

/* Прочитать матрицу n x n из файла. 0 - ок, -1 - нет файла, -2 - мало чисел. */
int read_matrix(double *a, int n, const char *name);

/* Напечатать левый верхний угол матрицы размером не больше m x m. */
void print_matrix(const double *a, int n, int m);

/* Норма матрицы: максимум по строкам суммы модулей. */
double matrix_norm(const double *a, int n);

/* Невязка ||A * X - E||. */
double residual(const double *a, const double *x, int n);

#endif
