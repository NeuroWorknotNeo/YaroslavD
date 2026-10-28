#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>

// Уравнение: 1 + exp(-2x^2) = alpha^2 * exp(-2x).
// Эквивалентная форма неподвижной точки (alpha != 0):
//     x = G(x) = ln|alpha| - ln(1 + exp(-2x^2)) / 2,
// |G'(x)| <= K = 1/2 на всей прямой.

const double EPS = 1e-12;
const double K = 0.5;
const int MAX_ITER = 10000;

double G(double x, double log_abs_alpha) {
    return log_abs_alpha - 0.5 * std::log1p(std::exp(-2.0 * x * x));
}

void solve(double alpha) {
    std::cout << std::defaultfloat << std::setprecision(12)
              << std::setw(14) << alpha;

    if (alpha == 0.0) {
        std::cout << std::setw(21) << "no solution"
                  << std::setw(8) << "-" << '\n';
        return;
    }

    const double log_abs_alpha = std::log(std::abs(alpha));
    double x = 0.0;
    int n = 0;

    while (true) {
        double new_x = G(x, log_abs_alpha);
        double step = std::abs(new_x - x);

        if (step / (1.0 - K) <= EPS) {
            break;
        }

        x = new_x;
        ++n;

        if (n > MAX_ITER) {
            std::cerr << "iteration limit exceeded for alpha = " << alpha << '\n';
            break;
        }
    }

    std::cout << std::fixed << std::setprecision(15)
              << std::setw(21) << x
              << std::setw(8) << n << '\n';
}

void print_header() {
    std::cout << std::setw(14) << "alpha"
              << std::setw(21) << "x"
              << std::setw(8) << "n" << '\n';
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        char* end = nullptr;
        double alpha = std::strtod(argv[1], &end);

        if (end == argv[1] || *end != '\0' || !std::isfinite(alpha)) {
            std::cerr << "alpha must be a number\n";
            return 1;
        }

        print_header();
        solve(alpha);
        return 0;
    }

    if (argc == 3) {
        char* end1 = nullptr;
        char* end2 = nullptr;
        long first = std::strtol(argv[1], &end1, 10);
        long last = std::strtol(argv[2], &end2, 10);

        if (end1 == argv[1] || end2 == argv[2] ||
            *end1 != '\0' || *end2 != '\0' || first >= last) {
            std::cerr << "bounds must be integers and first < last\n";
            return 1;
        }

        print_header();
        for (long alpha = first; alpha <= last; ++alpha) {
            solve(static_cast<double>(alpha));
        }
        return 0;
    }

    std::cerr << "Usage: iterative_solver alpha\n"
              << "   or: iterative_solver first_integer last_integer\n";
    return 1;
}
