#include <stdio.h>
#include <math.h>
#include "matrix.h"

/* Формулы из задания. Здесь i и j нумеруются с единицы. */
static double formula(int n, int k, int i, int j)
{
    int mx = (i > j ? i : j);

    switch (k) {
        case 1: return n - mx + 1;
        case 2: return mx;
        case 3: return fabs(i - j);
        case 4: return 1.0 / (i + j - 1);
    }
    return 0;
}

void fill_matrix(double *a, int n, int k)
{
    int i, j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            a[i * n + j] = formula(n, k, i + 1, j + 1);
}

int read_matrix(double *a, int n, const char *name)
{
    FILE *fp;
    int i;

    fp = fopen(name, "r");
    if (!fp)
        return -1;

    for (i = 0; i < n * n; i++) {
        if (fscanf(fp, "%lf", a + i) != 1) {
            fclose(fp);
            return -2;
        }
    }

    fclose(fp);
    return 0;
}

void print_matrix(const double *a, int n, int m)
{
    int i, j, p = (n < m ? n : m);

    for (i = 0; i < p; i++) {
        for (j = 0; j < p; j++)
            printf(" %10.3e", a[i * n + j]);
        printf("\n");
    }
}

double matrix_norm(const double *a, int n)
{
    int i, j;
    double max = 0;

    for (i = 0; i < n; i++) {
        double sum = 0;
        for (j = 0; j < n; j++)
            sum += fabs(a[i * n + j]);
        if (sum > max)
            max = sum;
    }
    return max;
}

double residual(const double *a, const double *x, int n)
{
    int i, j, k;
    double max = 0;

    for (i = 0; i < n; i++) {
        double sum = 0;
        for (j = 0; j < n; j++) {
            double s = 0;
            for (k = 0; k < n; k++)
                s += a[i * n + k] * x[k * n + j];
            sum += fabs(s - (i == j ? 1.0 : 0.0));
        }
        if (sum > max)
            max = sum;
    }
    return max;
}
