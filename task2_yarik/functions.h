#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/* Тип указателя на приближаемую функцию одной переменной */
using func_t = double (*)(double);

/* Возвращает указатель на функцию f_k по её номеру k = 0..6 */
func_t get_function(int k);

/* Текстовое представление функции f_k (для вывода в окне) */
const char *get_function_description(int k);

#endif /* FUNCTIONS_H */
