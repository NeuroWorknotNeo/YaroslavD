#ifndef PARABOLIC_EXTRAPOLATION_H
#define PARABOLIC_EXTRAPOLATION_H

#include "functions.h"

/* Задача 46. Интерполяция параболическими сплайнами с определением
 * недостающих граничных условий при помощи экстраполяции в приграничных
 * узлах.
 *
 * Схема построения. Кроме узлов интерполяции x_1 < ... < x_n вводятся
 * "полуузлы" (точки склейки)
 *   xi_1 = x_1 - (x_2 - x_1)/2,  xi_i = (x_{i-1} + x_i)/2 (i = 2..n),
 *   xi_{n+1} = x_n + (x_n - x_{n-1})/2.
 * На каждом отрезке [xi_i, xi_{i+1}] (он содержит ровно один узел x_i)
 * строится квадратный трёхчлен
 *   P_i(x) = c_0 + c_1 t + c_2 t^2,  t = x - xi_i,
 * причём P_i(x_i) = f_i, а значения в точках склейки v_i = S(xi_i)
 * являются неизвестными. Непрерывность S' в точках xi_2, ..., xi_n даёт
 * n-1 уравнений на n+1 неизвестных v_1, ..., v_{n+1}; получается
 * трёхдиагональная система, которая решается методом прогонки.
 *
 * Недостающие два уравнения берутся из экстраполяции в приграничных узлах:
 * значения v_1 и v_{n+1} в крайних точках склейки, лежащих вне [x_1, x_n],
 * вычисляются экстраполяцией интерполяционного многочлена, построенного по
 * трём ближайшим узлам (x_1, x_2, x_3 слева и x_{n-2}, x_{n-1}, x_n справа).
 *
 * n        -- число узлов (n >= 2);
 * x_nodes  -- массив узлов длины n (строго возрастающий);
 * f_values -- массив значений функции в узлах, длина n;
 * coeffs   -- выходной массив длины 3*n с коэффициентами трёхчленов;
 * xi_nodes -- рабочий массив длины n+1 (точки склейки);
 * v_values -- рабочий массив длины n+1 (значения сплайна в точках склейки).
 *
 * Возвращает 0 при успехе, -1 при ошибке (тогда coeffs[0] = NaN).
 */
int make_parabolic_extrapolation_coefficients(int n, const double *x_nodes,
                                              const double *f_values,
                                              double *coeffs, double *xi_nodes,
                                              double *v_values);

/* Вычисление значения параболического сплайна в точке x_eval */
double calculate_parabolic_extrapolation_value(double x_eval, double a,
                                               double b, int n,
                                               const double *x_nodes,
                                               const double *coeffs);

/* Погрешность f(x) - S(x) в точке x_eval */
double calculate_parabolic_extrapolation_discrepancy(double x_eval,
                                                     func_t func, double a,
                                                     double b, int n,
                                                     const double *x_nodes,
                                                     const double *coeffs);

#endif /* PARABOLIC_EXTRAPOLATION_H */
