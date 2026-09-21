#ifndef FUNCTIONS2D_H
#define FUNCTIONS2D_H

/* Тип указателя на приближаемую функцию двух переменных */
using func2_t = double (*)(double, double);

/* Число реализованных функций */
const int FUNCTIONS2D_COUNT = 8;

/* Функция f_k по её номеру k = 0..7 */
func2_t get_function2d(int k);

/* Текстовое представление функции f_k */
const char *get_function2d_description(int k);

#endif /* FUNCTIONS2D_H */
