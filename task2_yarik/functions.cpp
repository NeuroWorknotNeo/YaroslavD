#include <cmath>

#include "functions.h"

/* Набор приближаемых функций из требований к заданию */

static double f_0(double x) {
    (void)x;
    return 1.;
}

static double f_1(double x) {
    return x;
}

static double f_2(double x) {
    return x * x;
}

static double f_3(double x) {
    return x * x * x;
}

static double f_4(double x) {
    return x * x * x * x;
}

static double f_5(double x) {
    return std::exp(x);
}

static double f_6(double x) {
    return 1. / (25. * x * x + 1.);
}

func_t get_function(int k) {
    func_t f[] = {f_0, f_1, f_2, f_3, f_4, f_5, f_6};
    if (k < 0 || static_cast<unsigned>(k) >= sizeof(f) / sizeof(f[0]))
        k = 0;
    return f[k];
}

const char *get_function_description(int k) {
    switch (k) {
    case 1:  return "k = 1  f(x) = x";
    case 2:  return "k = 2  f(x) = x^2";
    case 3:  return "k = 3  f(x) = x^3";
    case 4:  return "k = 4  f(x) = x^4";
    case 5:  return "k = 5  f(x) = e^x";
    case 6:  return "k = 6  f(x) = 1/(25x^2 + 1)";
    default: return "k = 0  f(x) = 1";
    }
}
