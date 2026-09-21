#include <algorithm>
#include <cmath>
#include <limits>

#include "parabolic_extrapolation.h"

/* Значение в точке t интерполяционного многочлена, построенного по трём
 * узлам (x0, f0), (x1, f1), (x2, f2). Используется для экстраполяции
 * значения функции в крайние точки склейки, лежащие вне [x_1, x_n]. */
static double extrapolate_by_three_nodes(double t, double x0, double x1,
                                         double x2, double f0, double f1,
                                         double f2) {
    double df01, df12, df012;

    df01 = (f1 - f0) / (x1 - x0);
    df12 = (f2 - f1) / (x2 - x1);
    df012 = (df12 - df01) / (x2 - x0);

    return f0 + (t - x0) * (df01 + (t - x1) * df012);
}

int make_parabolic_extrapolation_coefficients(int n, const double *x_nodes,
                                              const double *f_values,
                                              double *coeffs, double *xi_nodes,
                                              double *v_values) {
    double h_left, z_left, u_left, h_right, z_right, u_right;
    double row_a, row_b, row_c, row_d, denom;
    double h, z, u, p, q, c2;
    double *alpha;
    int size;

    if (n < 2 || !x_nodes || !f_values || !coeffs || !xi_nodes || !v_values) {
        if (coeffs && n >= 1)
            coeffs[0] = std::numeric_limits<double>::quiet_NaN();
        return -1;
    }

    /* Узлы должны строго возрастать */
    for (int i = 0; i < n - 1; i++) {
        if (x_nodes[i + 1] - x_nodes[i] <= std::numeric_limits<double>::epsilon()) {
            coeffs[0] = std::numeric_limits<double>::quiet_NaN();
            return -1;
        }
    }

    /* Точки склейки: середины между узлами плюс два "полушага" наружу */
    xi_nodes[0] = x_nodes[0] - 0.5 * (x_nodes[1] - x_nodes[0]);
    for (int i = 1; i < n; i++)
        xi_nodes[i] = 0.5 * (x_nodes[i - 1] + x_nodes[i]);
    xi_nodes[n] = x_nodes[n - 1] + 0.5 * (x_nodes[n - 1] - x_nodes[n - 2]);

    size = n + 1; /* число неизвестных v_1, ..., v_{n+1} */

    /* Прогонка. Коэффициенты строк вычисляются на ходу, поэтому хранить
     * саму матрицу не нужно; alpha временно размещается в массиве coeffs
     * (в нём 3n >= n+1 элементов), beta -- в массиве v_values. */
    alpha = coeffs;

    /* Строка 0: граничное условие слева -- значение в xi_1 получено
     * экстраполяцией по ближайшим узлам */
    if (n >= 3)
        row_d = extrapolate_by_three_nodes(xi_nodes[0], x_nodes[0], x_nodes[1],
                                           x_nodes[2], f_values[0], f_values[1],
                                           f_values[2]);
    else /* n == 2: экстраполируем линейно по двум узлам */
        row_d = f_values[0] + (xi_nodes[0] - x_nodes[0]) *
                                  (f_values[1] - f_values[0]) /
                                  (x_nodes[1] - x_nodes[0]);
    alpha[0] = 0.;      /* -c/b при c = 0, b = 1 */
    v_values[0] = row_d; /* beta_0 */

    /* Строки 1..n-1: непрерывность производной сплайна в точках xi_2..xi_n */
    for (int i = 1; i <= n - 1; i++) {
        h_left = x_nodes[i - 1] - xi_nodes[i - 1];
        z_left = xi_nodes[i] - xi_nodes[i - 1];
        u_left = xi_nodes[i] - x_nodes[i - 1];

        h_right = x_nodes[i] - xi_nodes[i];
        z_right = xi_nodes[i + 1] - xi_nodes[i];
        u_right = xi_nodes[i + 1] - x_nodes[i];

        row_a = z_left / (h_left * u_left) -
                (2. * z_left - h_left) / (z_left * u_left);
        row_b = (2. * z_left - h_left) / (z_left * u_left) +
                z_right / (h_right * u_right) - h_right / (z_right * u_right);
        row_c = h_right / (z_right * u_right);
        row_d = z_left * f_values[i - 1] / (h_left * u_left) +
                z_right * f_values[i] / (h_right * u_right);

        denom = row_b + row_a * alpha[i - 1];
        if (std::fabs(denom) < std::numeric_limits<double>::epsilon()) {
            coeffs[0] = std::numeric_limits<double>::quiet_NaN();
            return -1;
        }
        alpha[i] = -row_c / denom;
        v_values[i] = (row_d - row_a * v_values[i - 1]) / denom;
    }

    /* Строка n: граничное условие справа -- значение в xi_{n+1} */
    if (n >= 3)
        row_d = extrapolate_by_three_nodes(xi_nodes[n], x_nodes[n - 3],
                                           x_nodes[n - 2], x_nodes[n - 1],
                                           f_values[n - 3], f_values[n - 2],
                                           f_values[n - 1]);
    else
        row_d = f_values[n - 1] + (xi_nodes[n] - x_nodes[n - 1]) *
                                      (f_values[n - 1] - f_values[n - 2]) /
                                      (x_nodes[n - 1] - x_nodes[n - 2]);
    /* row_a = 0, row_b = 1 (уравнение v_{n+1} = row_d) */
    v_values[n] = row_d;

    /* Обратный ход прогонки */
    for (int i = size - 2; i >= 0; i--)
        v_values[i] = alpha[i] * v_values[i + 1] + v_values[i];

    /* Коэффициенты квадратных трёхчленов на отрезках [xi_i, xi_{i+1}] */
    for (int i = 0; i < n; i++) {
        h = x_nodes[i] - xi_nodes[i];
        z = xi_nodes[i + 1] - xi_nodes[i];
        u = xi_nodes[i + 1] - x_nodes[i];

        p = (f_values[i] - v_values[i]) / h;
        q = (v_values[i + 1] - v_values[i]) / z;
        c2 = (q - p) / u;

        coeffs[3 * i + 0] = v_values[i];
        coeffs[3 * i + 1] = p - c2 * h;
        coeffs[3 * i + 2] = c2;
    }

    return 0;
}

double calculate_parabolic_extrapolation_value(double x_eval, double a,
                                               double b, int n,
                                               const double *x_nodes,
                                               const double *coeffs) {
    const double *c;
    double xi_left, t;
    int idx, i;

    (void)a;
    (void)b;

    if (n < 2 || !x_nodes || !coeffs || std::isnan(coeffs[0]))
        return std::numeric_limits<double>::quiet_NaN();

    /* Отрезок склейки [xi_i, xi_{i+1}] симметричен относительно узла x_i,
     * поэтому нужный кусок -- тот, чей узел ближе всего к точке */
    idx = static_cast<int>(std::upper_bound(x_nodes, x_nodes + n, x_eval) - x_nodes);
    if (idx == 0)
        i = 0;
    else if (idx == n)
        i = n - 1;
    else
        i = (x_eval - x_nodes[idx - 1] <= x_nodes[idx] - x_eval) ? idx - 1 : idx;

    xi_left = (i == 0) ? x_nodes[0] - 0.5 * (x_nodes[1] - x_nodes[0])
                       : 0.5 * (x_nodes[i - 1] + x_nodes[i]);

    t = x_eval - xi_left;
    c = coeffs + 3 * i;
    return c[0] + t * (c[1] + t * c[2]);
}

double calculate_parabolic_extrapolation_discrepancy(double x_eval,
                                                     func_t func, double a,
                                                     double b, int n,
                                                     const double *x_nodes,
                                                     const double *coeffs) {
    double approx;

    if (!func)
        return std::numeric_limits<double>::quiet_NaN();

    approx = calculate_parabolic_extrapolation_value(x_eval, a, b, n, x_nodes,
                                                     coeffs);
    if (std::isnan(approx))
        return std::numeric_limits<double>::quiet_NaN();

    return func(x_eval) - approx;
}
