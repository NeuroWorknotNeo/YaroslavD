#include <math.h>
#include "inverse.h"

static void swap_rows(double *a, int n, int i, int j)
{
    int k;
    double t;

    for (k = 0; k < n; k++) {
        t = a[i * n + k];
        a[i * n + k] = a[j * n + k];
        a[j * n + k] = t;
    }
}

static void swap_cols(double *a, int n, int i, int j)
{
    int k;
    double t;

    for (k = 0; k < n; k++) {
        t = a[k * n + i];
        a[k * n + i] = a[k * n + j];
        a[k * n + j] = t;
    }
}

int inverse_matrix(double *a, double *x, int *col, int n, double norm)
{
    int i, j, k;
    double eps = 1e-15 * norm;

    /* x = единичная матрица, col = (0, 1, 2, ...) */
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            x[i * n + j] = (i == j ? 1.0 : 0.0);
        col[i] = i;
    }

    for (k = 0; k < n; k++) {
        int pi = k, pj = k;
        double best = fabs(a[k * n + k]), inv;

        /* 1. Ищем самый большой по модулю элемент во всём нижнем правом углу. */
        for (i = k; i < n; i++)
            for (j = k; j < n; j++)
                if (fabs(a[i * n + j]) > best) {
                    best = fabs(a[i * n + j]);
                    pi = i;
                    pj = j;
                }

        if (best <= eps)
            return -1;                  /* матрица вырождена */

        /* 2. Двигаем его в угол: строки меняем и в a, и в x, столбцы - только в a. */
        if (pi != k) {
            swap_rows(a, n, k, pi);
            swap_rows(x, n, k, pi);
        }
        if (pj != k) {
            int t;
            swap_cols(a, n, k, pj);
            t = col[k];
            col[k] = col[pj];
            col[pj] = t;
        }

        /* 3. Делим строку k на главный элемент, чтобы на диагонали стала единица. */
        inv = 1.0 / a[k * n + k];
        for (j = k; j < n; j++)
            a[k * n + j] *= inv;
        for (j = 0; j < n; j++)
            x[k * n + j] *= inv;

        /* 4. Вычитаем строку k из всех остальных строк, чтобы в столбце k были нули. */
        for (i = 0; i < n; i++) {
            double c = a[i * n + k];

            if (i == k || c == 0.0)
                continue;
            for (j = k; j < n; j++)
                a[i * n + j] -= c * a[k * n + j];
            for (j = 0; j < n; j++)
                x[i * n + j] -= c * x[k * n + j];
        }
    }

    /* 5. Из-за перестановок столбцов строки ответа перепутаны:
          строка k матрицы x - это на самом деле строка col[k] обратной матрицы. */
    for (i = 0; i < n; i++) {
        while (col[i] != i) {
            int t = col[i];
            swap_rows(x, n, i, t);
            col[i] = col[t];
            col[t] = t;
        }
    }

    return 0;
}
