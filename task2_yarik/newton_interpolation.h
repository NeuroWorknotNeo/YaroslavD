#ifndef NEWTON_INTERPOLATION_H
#define NEWTON_INTERPOLATION_H

#include "functions.h"

/* Задача 1. Интерполяционная формула Ньютона.
 *
 * Строится один многочлен степени n-1, проходящий через все узлы:
 *   P(x) = a_0 + a_1 (x-x_1) + a_2 (x-x_1)(x-x_2) + ... ,
 * где a_i = f[x_1, ..., x_{i+1}] -- разделённые разности.
 *
 * n        -- число узлов интерполяции (n >= 1);
 * x_nodes  -- массив узлов длины n (строго возрастающий);
 * f_values -- массив значений функции в узлах, длина n;
 * coeffs   -- выходной массив длины n для разделённых разностей.
 *
 * Возвращает 0 при успехе, -1 при ошибке (тогда coeffs[0] = NaN).
 */
int make_newton_coefficients(int n, const double *x_nodes,
                             const double *f_values, double *coeffs);

/* Вычисление значения интерполяционного многочлена Ньютона в точке
 * по схеме Горнера.
 *
 * x_eval -- точка вычисления;
 * a, b   -- отрезок, на котором построено приближение (не используется,
 *           присутствует по требованиям к оформлению подпрограммы);
 * n, x_nodes, coeffs -- как выше.
 */
double calculate_newton_value(double x_eval, double a, double b, int n,
                              const double *x_nodes, const double *coeffs);

/* Погрешность |f(x) - P(x)| в точке x_eval */
double calculate_newton_discrepancy(double x_eval, func_t func, double a,
                                    double b, int n, const double *x_nodes,
                                    const double *coeffs);

#endif /* NEWTON_INTERPOLATION_H */
