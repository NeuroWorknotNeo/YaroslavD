#include <cmath>
#include <limits>

#include "newton_interpolation.h"

int make_newton_coefficients(int n, const double *x_nodes,
                             const double *f_values, double *coeffs) {
    double dx;

    if (n < 1 || !x_nodes || !f_values || !coeffs) {
        if (coeffs && n >= 1)
            coeffs[0] = std::numeric_limits<double>::quiet_NaN();
        return -1;
    }

    /* Нулевой столбец таблицы разделённых разностей -- значения функции */
    for (int i = 0; i < n; i++)
        coeffs[i] = f_values[i];

    /* Пересчёт "на месте": после j-го прохода coeffs[i] = f[x_{i-j}, ..., x_i] */
    for (int j = 1; j < n; j++) {
        for (int i = n - 1; i >= j; i--) {
            dx = x_nodes[i] - x_nodes[i - j];
            if (std::fabs(dx) < std::numeric_limits<double>::epsilon()) {
                coeffs[0] = std::numeric_limits<double>::quiet_NaN();
                return -1;
            }
            coeffs[i] = (coeffs[i] - coeffs[i - 1]) / dx;
        }
    }

    return 0;
}

double calculate_newton_value(double x_eval, double a, double b, int n,
                              const double *x_nodes, const double *coeffs) {
    double value;

    (void)a;
    (void)b;

    if (n < 1 || !x_nodes || !coeffs || std::isnan(coeffs[0]))
        return std::numeric_limits<double>::quiet_NaN();

    /* Схема Горнера для формы Ньютона:
     * P(x) = a_0 + (x-x_1)(a_1 + (x-x_2)(a_2 + ...)) */
    value = coeffs[n - 1];
    for (int i = n - 2; i >= 0; i--)
        value = coeffs[i] + (x_eval - x_nodes[i]) * value;

    return value;
}

double calculate_newton_discrepancy(double x_eval, func_t func, double a,
                                    double b, int n, const double *x_nodes,
                                    const double *coeffs) {
    double approx;

    if (!func)
        return std::numeric_limits<double>::quiet_NaN();

    approx = calculate_newton_value(x_eval, a, b, n, x_nodes, coeffs);
    if (std::isnan(approx))
        return std::numeric_limits<double>::quiet_NaN();

    return func(x_eval) - approx;
}
