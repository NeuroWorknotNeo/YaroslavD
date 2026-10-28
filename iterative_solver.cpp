#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>

const double EPS = 1e-12;
const double K = 0.75;

double sign(double y) {
    if (y > 0.0) return 1.0;
    if (y < 0.0) return -1.0;
    return 0.0;
}

void solve(double alpha) {
    double x = 0.0;
    double y = 0.0;
    int n = 0;

    while (true) {
        double new_x = -(std::tanh(x) + sign(y) * (1.0 - std::exp(-std::abs(y)))) / 4.0;
        double new_y = (alpha - std::sin(x) - 1.0 / (1.0 + y * y)) / 2.0;
        double step = std::abs(new_x - x) + std::abs(new_y - y);

        if (step / (1.0 - K) <= EPS) {
            break;
        }

        x = new_x;
        y = new_y;
        ++n;
    }

    std::cout << std::defaultfloat << std::setprecision(12)
              << std::setw(14) << alpha
              << std::fixed << std::setprecision(15)
              << std::setw(21) << x
              << std::setw(21) << y
              << std::setw(8) << n << '\n';
}

void print_header() {
    std::cout << std::setw(14) << "alpha"
              << std::setw(21) << "x"
              << std::setw(21) << "y"
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
