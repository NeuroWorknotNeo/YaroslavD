#include <cmath>

#include "functions2d.h"

/* Стандартный набор приближаемых функций двух переменных */

static double f_0(double x, double y) {
    (void)x;
    (void)y;
    return 1.;
}

static double f_1(double x, double y) {
    (void)y;
    return x;
}

static double f_2(double x, double y) {
    (void)x;
    return y;
}

static double f_3(double x, double y) {
    return x + y;
}

static double f_4(double x, double y) {
    return std::sqrt(x * x + y * y);
}

static double f_5(double x, double y) {
    return x * x + y * y;
}

static double f_6(double x, double y) {
    return std::exp(x * x - y * y);
}

static double f_7(double x, double y) {
    return 1. / (25. * (x * x + y * y) + 1.);
}

func2_t get_function2d(int k) {
    func2_t f[FUNCTIONS2D_COUNT] = {f_0, f_1, f_2, f_3, f_4, f_5, f_6, f_7};
    if (k < 0 || k >= FUNCTIONS2D_COUNT)
        k = 0;
    return f[k];
}

const char *get_function2d_description(int k) {
    switch (k) {
    case 1:  return "k = 1  f(x,y) = x";
    case 2:  return "k = 2  f(x,y) = y";
    case 3:  return "k = 3  f(x,y) = x + y";
    case 4:  return "k = 4  f(x,y) = sqrt(x^2 + y^2)";
    case 5:  return "k = 5  f(x,y) = x^2 + y^2";
    case 6:  return "k = 6  f(x,y) = e^(x^2 - y^2)";
    case 7:  return "k = 7  f(x,y) = 1/(25(x^2 + y^2) + 1)";
    default: return "k = 0  f(x,y) = 1";
    }
}
