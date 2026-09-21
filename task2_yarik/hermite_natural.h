#ifndef HERMITE_NATURAL_H
#define HERMITE_NATURAL_H

#include "functions.h"

/* Задача 13. Кусочная интерполяция кубическими многочленами Эрмита
 * с определением недостающих граничных условий из естественных граничных
 * условий (S''(x_1) = S''(x_n) = 0).
 *
 * На каждом отрезке [x_i, x_{i+1}] строится кубический многочлен Эрмита,
 * склеенный с соседями по значению и по первой производной:
 *   P_i(x) = c_0 + c_1 t + c_2 t^2 + c_3 t^3,  t = x - x_i.
 * Значения производных d_i во внутренних узлах вычисляются локально по
 * разделённым разностям:
 *   d_i = f[x_{i-1}, x_i] + f[x_i, x_{i+1}] - f[x_{i-1}, x_{i+1}]
 * (на равномерной сетке это (f_{i+1} - f_{i-1}) / (2h)).
 * В граничных узлах x_1 и x_n данных для этой формулы не хватает, поэтому
 * недостающие d_1 и d_n находятся из естественных условий нулевой второй
 * производной на концах отрезка.
 *
 * n         -- число узлов (n >= 2);
 * x_nodes   -- массив узлов длины n (строго возрастающий);
 * f_values  -- массив значений функции в узлах, длина n;
 * coeffs    -- выходной массив длины 4*(n-1) с коэффициентами многочленов;
 * d_values  -- рабочий массив длины n (значения производных в узлах).
 *
 * Возвращает 0 при успехе, -1 при ошибке (тогда coeffs[0] = NaN).
 */
int make_hermite_natural_coefficients(int n, const double *x_nodes,
                                      const double *f_values, double *coeffs,
                                      double *d_values);

/* Вычисление значения кусочно-кубической функции в точке x_eval.
 * Отрезок [a, b] и массив узлов используются для поиска нужного куска. */
double calculate_hermite_natural_value(double x_eval, double a, double b,
                                       int n, const double *x_nodes,
                                       const double *coeffs);

/* Погрешность |f(x) - S(x)| в точке x_eval */
double calculate_hermite_natural_discrepancy(double x_eval, func_t func,
                                             double a, double b, int n,
                                             const double *x_nodes,
                                             const double *coeffs);

#endif /* HERMITE_NATURAL_H */
