#include <algorithm>
#include <cmath>
#include <limits>

#include "hermite_natural.h"

int make_hermite_natural_coefficients(int n, const double *x_nodes,
                                      const double *f_values, double *coeffs,
                                      double *d_values) {
    double h_left, h_right, df_left, df_right, h, df;

    if (n < 2 || !x_nodes || !f_values || !coeffs || !d_values) {
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

    if (n == 2) {
        /* Один отрезок: естественные условия S''= 0 на обоих концах
         * превращают кубический многочлен в линейный */
        df = (f_values[1] - f_values[0]) / (x_nodes[1] - x_nodes[0]);
        d_values[0] = df;
        d_values[1] = df;
    } else {
        /* Производные во внутренних узлах -- по трём соседним значениям */
        for (int i = 1; i < n - 1; i++) {
            h_left = x_nodes[i] - x_nodes[i - 1];
            h_right = x_nodes[i + 1] - x_nodes[i];
            df_left = (f_values[i] - f_values[i - 1]) / h_left;
            df_right = (f_values[i + 1] - f_values[i]) / h_right;
            d_values[i] = df_left + df_right -
                          (f_values[i + 1] - f_values[i - 1]) / (h_left + h_right);
        }

        /* Естественные граничные условия.
         * P_1''(x_1) = 0  =>  3 f[x_1,x_2] - 2 d_1 - d_2 = 0;
         * P_{n-1}''(x_n) = 0  =>  3 f[x_{n-1},x_n] - 2 d_n - d_{n-1} = 0. */
        df_left = (f_values[1] - f_values[0]) / (x_nodes[1] - x_nodes[0]);
        d_values[0] = 0.5 * (3. * df_left - d_values[1]);

        df_right = (f_values[n - 1] - f_values[n - 2]) /
                   (x_nodes[n - 1] - x_nodes[n - 2]);
        d_values[n - 1] = 0.5 * (3. * df_right - d_values[n - 2]);
    }

    /* Коэффициенты кубических многочленов Эрмита на каждом отрезке */
    for (int i = 0; i < n - 1; i++) {
        h = x_nodes[i + 1] - x_nodes[i];
        df = (f_values[i + 1] - f_values[i]) / h;

        coeffs[4 * i + 0] = f_values[i];
        coeffs[4 * i + 1] = d_values[i];
        coeffs[4 * i + 2] = (3. * df - 2. * d_values[i] - d_values[i + 1]) / h;
        coeffs[4 * i + 3] = (d_values[i] + d_values[i + 1] - 2. * df) / (h * h);
    }

    return 0;
}

double calculate_hermite_natural_value(double x_eval, double a, double b,
                                       int n, const double *x_nodes,
                                       const double *coeffs) {
    const double *c;
    double t;
    int i;

    (void)a;
    (void)b;

    if (n < 2 || !x_nodes || !coeffs || std::isnan(coeffs[0]))
        return std::numeric_limits<double>::quiet_NaN();

    /* Двоичный поиск отрезка [x_i, x_{i+1}], содержащего точку;
     * вне [x_1, x_n] используется крайний многочлен (экстраполяция) */
    i = static_cast<int>(std::upper_bound(x_nodes, x_nodes + n, x_eval) - x_nodes) - 1;
    if (i < 0)
        i = 0;
    if (i > n - 2)
        i = n - 2;

    t = x_eval - x_nodes[i];
    c = coeffs + 4 * i;
    return c[0] + t * (c[1] + t * (c[2] + t * c[3]));
}

double calculate_hermite_natural_discrepancy(double x_eval, func_t func,
                                             double a, double b, int n,
                                             const double *x_nodes,
                                             const double *coeffs) {
    double approx;

    if (!func)
        return std::numeric_limits<double>::quiet_NaN();

    approx = calculate_hermite_natural_value(x_eval, a, b, n, x_nodes, coeffs);
    if (std::isnan(approx))
        return std::numeric_limits<double>::quiet_NaN();

    return func(x_eval) - approx;
}
