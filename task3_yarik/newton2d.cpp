#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

#include "newton2d.h"

void make_chebyshev_nodes(int n, double a, double b, double *nodes) {
    double center = 0.5 * (a + b);
    double half = 0.5 * (b - a);

    if (n < 1 || !nodes)
        return;

    if (n == 1) {
        nodes[0] = center;
        return;
    }

    /* cos(...) убывает по i, поэтому заполняем массив с конца,
     * чтобы узлы шли по возрастанию */
    for (int i = 0; i < n; i++)
        nodes[n - 1 - i] = center + half * std::cos(M_PI * (i + 0.5) / n);
}

void reorder_nodes_leja(int n, double *nodes) {
    std::vector<double> weight;
    double best_value, center, lo, hi;
    int best;

    if (n < 2 || !nodes)
        return;

    /* Первым берём узел, наиболее удалённый от середины набора узлов */
    lo = hi = nodes[0];
    for (int i = 1; i < n; i++) {
        lo = std::min(lo, nodes[i]);
        hi = std::max(hi, nodes[i]);
    }
    center = 0.5 * (lo + hi);

    weight.assign(n, 0.);
    for (int i = 0; i < n; i++)
        weight[i] = std::fabs(nodes[i] - center);

    for (int k = 0; k < n - 1; k++) {
        best = k;
        best_value = weight[k];
        for (int i = k + 1; i < n; i++) {
            if (weight[i] > best_value) {
                best_value = weight[i];
                best = i;
            }
        }
        std::swap(nodes[k], nodes[best]);
        std::swap(weight[k], weight[best]);

        /* Домножаем веса оставшихся узлов на расстояние до выбранного */
        for (int i = k + 1; i < n; i++)
            weight[i] *= std::fabs(nodes[i] - nodes[k]);
    }
}

int make_newton2d_coefficients(int nx, int ny, const double *x_nodes,
                               const double *y_nodes, const double *f_values,
                               double *coeffs) {
    double dx;

    if (nx < 1 || ny < 1 || !x_nodes || !y_nodes || !f_values || !coeffs) {
        if (coeffs)
            coeffs[0] = std::numeric_limits<double>::quiet_NaN();
        return -1;
    }

    for (int i = 0; i < nx * ny; i++)
        coeffs[i] = f_values[i];

    /* Шаг 1. Разделённые разности по переменной x: для каждого
     * фиксированного j столбец coeffs[*][j] обрабатывается так же,
     * как одномерная таблица разделённых разностей. */
    for (int level = 1; level < nx; level++) {
        for (int i = nx - 1; i >= level; i--) {
            dx = x_nodes[i] - x_nodes[i - level];
            if (std::fabs(dx) < std::numeric_limits<double>::epsilon()) {
                coeffs[0] = std::numeric_limits<double>::quiet_NaN();
                return -1;
            }
            for (int j = 0; j < ny; j++)
                coeffs[i * ny + j] =
                    (coeffs[i * ny + j] - coeffs[(i - 1) * ny + j]) / dx;
        }
    }

    /* Шаг 2. Разделённые разности по переменной y: теперь то же самое
     * делается внутри каждой строки. Результат -- смешанные разделённые
     * разности c_ij = f[x_1,...,x_i; y_1,...,y_j]. */
    for (int i = 0; i < nx; i++) {
        for (int level = 1; level < ny; level++) {
            for (int j = ny - 1; j >= level; j--) {
                dx = y_nodes[j] - y_nodes[j - level];
                if (std::fabs(dx) < std::numeric_limits<double>::epsilon()) {
                    coeffs[0] = std::numeric_limits<double>::quiet_NaN();
                    return -1;
                }
                coeffs[i * ny + j] =
                    (coeffs[i * ny + j] - coeffs[i * ny + (j - 1)]) / dx;
            }
        }
    }

    return 0;
}

double calculate_newton2d_value(double px, double py, int nx, int ny,
                                const double *x_nodes, const double *y_nodes,
                                const double *coeffs) {
    double result, inner;

    if (nx < 1 || ny < 1 || !x_nodes || !y_nodes || !coeffs ||
        std::isnan(coeffs[0]))
        return std::numeric_limits<double>::quiet_NaN();

    /* Внешняя схема Горнера по x; коэффициентами служат значения
     * внутренних многочленов по y:
     *     P(x,y) = g_1(y) + (x-x_1)( g_2(y) + (x-x_2)( ... ) ),
     *     g_i(y) = c_i1 + (y-y_1)( c_i2 + (y-y_2)( ... ) ). */
    result = coeffs[(nx - 1) * ny + ny - 1];
    for (int j = ny - 2; j >= 0; j--)
        result = result * (py - y_nodes[j]) + coeffs[(nx - 1) * ny + j];

    for (int i = nx - 2; i >= 0; i--) {
        inner = coeffs[i * ny + ny - 1];
        for (int j = ny - 2; j >= 0; j--)
            inner = inner * (py - y_nodes[j]) + coeffs[i * ny + j];

        result = result * (px - x_nodes[i]) + inner;
    }

    return result;
}

double calculate_newton2d_discrepancy(double px, double py, func2_t func,
                                      int nx, int ny, const double *x_nodes,
                                      const double *y_nodes,
                                      const double *coeffs) {
    double approx;

    if (!func)
        return std::numeric_limits<double>::quiet_NaN();

    approx = calculate_newton2d_value(px, py, nx, ny, x_nodes, y_nodes, coeffs);
    if (std::isnan(approx))
        return std::numeric_limits<double>::quiet_NaN();

    return func(px, py) - approx;
}
