#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "inverse.h"

int main(int argc, char *argv[])
{
    int n, m, k, res, *col;
    double *a, *b, *x, norm, r, t;
    const char *name = 0;

    if (argc != 4 && argc != 5) {
        printf("Использование: %s n m k [файл]\n", argv[0]);
        return 1;
    }
    if (sscanf(argv[1], "%d", &n) != 1 || sscanf(argv[2], "%d", &m) != 1 ||
        sscanf(argv[3], "%d", &k) != 1 || n <= 0 || m < 0 || k < 0 || k > 4) {
        printf("Неверные аргументы\n");
        return 1;
    }
    if (k == 0 && argc != 5) {
        printf("При k = 0 нужно указать имя файла\n");
        return 1;
    }
    if (k == 0)
        name = argv[4];

    a = new double[n * n];          /* исходная матрица */
    b = new double[n * n];          /* её копия для невязки */
    x = new double[n * n];          /* обратная матрица */
    col = new int[n];

    if (k == 0) {
        res = read_matrix(a, n, name);
        if (res < 0) {
            printf("Не удалось прочитать матрицу из файла %s\n", name);
            delete[] a; delete[] b; delete[] x; delete[] col;
            return 2;
        }
    } else {
        fill_matrix(a, n, k);
    }

    for (int i = 0; i < n * n; i++)
        b[i] = a[i];

    printf("Исходная матрица:\n");
    print_matrix(a, n, m);

    norm = matrix_norm(a, n);
    t = clock();
    res = inverse_matrix(a, x, col, n, norm);
    t = (clock() - t) / CLOCKS_PER_SEC;

    if (res < 0) {
        printf("Матрица вырождена, обратной не существует\n");
        delete[] a; delete[] b; delete[] x; delete[] col;
        return 3;
    }

    printf("Обратная матрица:\n");
    print_matrix(x, n, m);

    r = residual(b, x, n);
    printf("Невязка ||A * A^(-1) - E|| = %e\n", r);
    printf("Время работы = %.2f с (n = %d, m = %d, k = %d)\n", t, n, m, k);

    delete[] a;
    delete[] b;
    delete[] x;
    delete[] col;
    return 0;
}
