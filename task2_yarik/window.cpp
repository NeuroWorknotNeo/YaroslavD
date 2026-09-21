#include <QPaintEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QString>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <limits>

#include "hermite_natural.h"
#include "newton_interpolation.h"
#include "parabolic_extrapolation.h"
#include "window.h"

static const char *mode_description(int mode) {
    switch (mode) {
    case 1:  return "f и многочлен Ньютона";
    case 2:  return "f и сплайн Эрмита";
    case 3:  return "f и параболический сплайн";
    case 4:  return "f и все три приближения";
    case 5:  return "погрешность многочлена Ньютона";
    case 6:  return "погрешность сплайна Эрмита";
    case 7:  return "погрешность параболического сплайна";
    case 8:  return "погрешности всех трёх методов";
    default: return "только функция f";
    }
}

Window::Window(QWidget *parent, double a_in, double b_in, int n_in, int k_in)
    : QWidget(parent), a(a_in), b(b_in), show_a(a_in), show_b(b_in), n(n_in),
      k(k_in) {
    function = get_function(k);
    calculate_max_abs_f();
    rebuild();
}

/* ---------- подготовка данных ---------- */

void Window::allocate_arrays() {
    x.reset(new double[n]);
    f.reset(new double[n]);

    if (n <= MAX_NEWTON_POINTS)
        newton_c.reset(new double[n]);
    else
        newton_c.reset();

    hermite_c.reset(new double[n >= 2 ? 4 * (n - 1) : 1]);
    parabolic_c.reset(new double[n >= 2 ? 3 * n : 1]);
}

void Window::calculate_points() {
    if (n == 1) {
        x[0] = 0.5 * (a + b);
        f[0] = function(x[0]);
        return;
    }

    for (int i = 0; i < n; i++) {
        x[i] = a + i * (b - a) / (n - 1);
        f[i] = function(x[i]);
    }

    /* Моделирование погрешности измерения в средней точке */
    if (p != 0)
        f[n / 2] += p * 0.1 * max_abs_f;
}

void Window::build_approximations() {
    std::clock_t start = std::clock();

    newton_ready = false;
    hermite_ready = false;
    parabolic_ready = false;

    /* Метод 1: интерполяционный многочлен Ньютона (только при n <= 50) */
    if (n >= 1 && n <= MAX_NEWTON_POINTS && newton_c)
        newton_ready = (make_newton_coefficients(n, x.get(), f.get(),
                                                 newton_c.get()) == 0) &&
                       !std::isnan(newton_c[0]);

    if (n >= 2) {
        /* Метод 2: кусочная интерполяция кубическими многочленами Эрмита
         * (дополнительный рабочий массив производных в узлах) */
        std::unique_ptr<double[]> d(new double[n]);
        hermite_ready = (make_hermite_natural_coefficients(
                             n, x.get(), f.get(), hermite_c.get(), d.get()) == 0) &&
                        !std::isnan(hermite_c[0]);

        /* Метод 3: интерполяция параболическими сплайнами
         * (рабочие массивы точек склейки и значений в них) */
        std::unique_ptr<double[]> xi(new double[n + 1]);
        std::unique_ptr<double[]> v(new double[n + 1]);
        parabolic_ready = (make_parabolic_extrapolation_coefficients(
                               n, x.get(), f.get(), parabolic_c.get(), xi.get(),
                               v.get()) == 0) &&
                          !std::isnan(parabolic_c[0]);
    }

    std::printf("n = %d: построение приближений заняло %.3f с\n", n,
                static_cast<double>(std::clock() - start) / CLOCKS_PER_SEC);
}

void Window::calculate_max_abs_f() {
    int points = 10000;
    double t, value;

    max_abs_f = std::fabs(function(a));
    for (int i = 1; i <= points; i++) {
        t = a + i * (b - a) / points;
        value = std::fabs(function(t));
        if (value > max_abs_f)
            max_abs_f = value;
    }
}

void Window::rebuild() {
    allocate_arrays();
    calculate_points();
    build_approximations();
    calculate_min_max();
}

/* ---------- значения приближений и погрешностей ---------- */

double Window::newton_value(double t) const {
    return calculate_newton_value(t, a, b, n, x.get(), newton_c.get());
}

double Window::hermite_value(double t) const {
    return calculate_hermite_natural_value(t, a, b, n, x.get(), hermite_c.get());
}

double Window::parabolic_value(double t) const {
    return calculate_parabolic_extrapolation_value(t, a, b, n, x.get(),
                                                   parabolic_c.get());
}

double Window::newton_error(double t) const {
    return calculate_newton_discrepancy(t, function, a, b, n, x.get(),
                                        newton_c.get());
}

double Window::hermite_error(double t) const {
    return calculate_hermite_natural_discrepancy(t, function, a, b, n, x.get(),
                                                 hermite_c.get());
}

double Window::parabolic_error(double t) const {
    return calculate_parabolic_extrapolation_discrepancy(t, function, a, b, n,
                                                         x.get(),
                                                         parabolic_c.get());
}

/* ---------- масштабирование ---------- */

void Window::calculate_min_max() {
    int points = (width() > 0 ? width() : 1000);
    double t, y;
    bool empty = true;

    min_y = 0.;
    max_y = 0.;

    for (int i = 0; i <= points; i++) {
        t = (i < points) ? show_a + i * (show_b - show_a) / points : show_b;

        double values[7];
        int count = 0;

        if (draw_function())
            values[count++] = function(t);
        if (draw_newton())
            values[count++] = newton_value(t);
        if (draw_hermite())
            values[count++] = hermite_value(t);
        if (draw_parabolic())
            values[count++] = parabolic_value(t);
        if (draw_newton_error())
            values[count++] = newton_error(t);
        if (draw_hermite_error())
            values[count++] = hermite_error(t);
        if (draw_parabolic_error())
            values[count++] = parabolic_error(t);

        for (int j = 0; j < count; j++) {
            y = values[j];
            if (std::isnan(y) || std::isinf(y))
                continue;
            if (empty) {
                min_y = max_y = y;
                empty = false;
            } else {
                min_y = std::min(min_y, y);
                max_y = std::max(max_y, y);
            }
        }
    }

    if (empty) {
        min_y = -1.;
        max_y = 1.;
    }

    max_abs_F = std::fmax(std::fabs(min_y), std::fabs(max_y));
    std::printf("max{|F|} = %.3e   (n = %d, k = %d, s = %d, p = %d, режим: %s)\n",
                max_abs_F, n, k, s, p, mode_description(mode));
    std::fflush(stdout);

    char buf[64];
    std::snprintf(buf, sizeof(buf), "max{|F|} = %.3e", max_abs_F);
    max_abs_F_changed(QString(buf));

    /* Небольшой отступ по вертикали, чтобы график не касался границ окна */
    double delta = 0.05 * (max_y - min_y);
    if (delta < std::numeric_limits<double>::epsilon())
        delta = (max_abs_F > 0 ? 0.05 * max_abs_F : 0.05);
    min_y -= delta;
    max_y += delta;
}

QPointF Window::l2g(double x_loc, double y_loc) const {
    double x_gl, y_gl;

    if (std::fabs(show_b - show_a) < std::numeric_limits<double>::epsilon())
        x_gl = 0.5 * width();
    else
        x_gl = (x_loc - show_a) / (show_b - show_a) * width();

    if (std::fabs(max_y - min_y) < std::numeric_limits<double>::epsilon())
        y_gl = 0.5 * height();
    else
        y_gl = (max_y - y_loc) / (max_y - min_y) * height();

    return QPointF(x_gl, y_gl);
}

/* ---------- обработка нажатий клавиш ---------- */

void Window::init_status_bar() {
    char buf[64];

    function_description_changed(QString(get_function_description(k)));
    std::snprintf(buf, sizeof(buf), "n = %d", n);
    number_of_points_changed(QString(buf));
    std::snprintf(buf, sizeof(buf), "s = %d", s);
    scale_changed(QString(buf));
    std::snprintf(buf, sizeof(buf), "p = %d", p);
    distortion_changed(QString(buf));
    std::snprintf(buf, sizeof(buf), "max{|F|} = %.3e", max_abs_F);
    max_abs_F_changed(QString(buf));
}

void Window::change_function() {
    char buf[64];

    k = (k + 1) % 7;
    function = get_function(k);
    p = 0;

    calculate_max_abs_f();
    rebuild();

    function_description_changed(QString(get_function_description(k)));
    std::snprintf(buf, sizeof(buf), "p = %d", p);
    distortion_changed(QString(buf));
    update();
}

void Window::change_graph() {
    mode = (mode + 1) % TOTAL_MODES;
    calculate_min_max();
    update();
}

void Window::increase_scale() {
    double center = 0.5 * (show_a + show_b);
    char buf[64];

    s++;
    show_a = center + 0.5 * (show_a - center);
    show_b = center + 0.5 * (show_b - center);

    std::snprintf(buf, sizeof(buf), "s = %d", s);
    scale_changed(QString(buf));
    calculate_min_max();
    update();
}

void Window::decrease_scale() {
    double center = 0.5 * (show_a + show_b);
    char buf[64];

    s--;
    show_a = center + 2. * (show_a - center);
    show_b = center + 2. * (show_b - center);

    std::snprintf(buf, sizeof(buf), "s = %d", s);
    scale_changed(QString(buf));
    calculate_min_max();
    update();
}

void Window::increase_points() {
    char buf[64];

    if (n > MAX_POINTS / 2) {
        std::printf("Дальнейшее увеличение n не выполняется: достигнут предел %d\n",
                    MAX_POINTS);
        return;
    }
    n *= 2;
    rebuild();

    std::snprintf(buf, sizeof(buf), "n = %d", n);
    number_of_points_changed(QString(buf));
    update();
}

void Window::decrease_points() {
    char buf[64];

    if (n <= 1)
        return;
    n /= 2;
    rebuild();

    std::snprintf(buf, sizeof(buf), "n = %d", n);
    number_of_points_changed(QString(buf));
    update();
}

void Window::add_distortion() {
    char buf[64];

    p++;
    calculate_points();
    build_approximations();
    calculate_min_max();

    std::snprintf(buf, sizeof(buf), "p = %d", p);
    distortion_changed(QString(buf));
    update();
}

void Window::subtract_distortion() {
    char buf[64];

    p--;
    calculate_points();
    build_approximations();
    calculate_min_max();

    std::snprintf(buf, sizeof(buf), "p = %d", p);
    distortion_changed(QString(buf));
    update();
}

/* ---------- рисование ---------- */

template <typename Function>
void Window::draw_graph(QPainter &painter, double left, double right,
                        Function func) {
    int points = (width() > 0 ? width() : 1000);
    double delta = (right - left) / points;
    double x1 = left, y1 = func(left), x2, y2;

    for (int i = 1; i <= points; i++) {
        x2 = (i < points) ? left + i * delta : right;
        y2 = func(x2);
        if (!std::isnan(y1) && !std::isnan(y2) && !std::isinf(y1) &&
            !std::isinf(y2))
            painter.drawLine(l2g(x1, y1), l2g(x2, y2));
        x1 = x2;
        y1 = y2;
    }
}

void Window::resizeEvent(QResizeEvent *event) {
    /* При изменении размера окна число точек выборки меняется,
     * поэтому масштаб по вертикали пересчитывается заново */
    QWidget::resizeEvent(event);
    calculate_min_max();
}

void Window::paintEvent(QPaintEvent *event) {
    (void)event;

    QPainter painter(this);
    QPen pen_axis(Qt::black, 1);
    QPen pen_function(Qt::red, 2);
    QPen pen_newton(Qt::blue, 2);
    QPen pen_hermite(Qt::darkGreen, 2);
    QPen pen_parabolic(Qt::magenta, 2);
    char buf[128];

    painter.fillRect(rect(), Qt::white);

    /* Оси координат */
    painter.setPen(pen_axis);
    if (min_y <= 0. && max_y >= 0.)
        painter.drawLine(l2g(show_a, 0.), l2g(show_b, 0.));
    if (show_a <= 0. && show_b >= 0.)
        painter.drawLine(l2g(0., min_y), l2g(0., max_y));

    if (draw_function()) {
        painter.setPen(pen_function);
        draw_graph(painter, show_a, show_b, [this](double t) { return function(t); });
    }
    if (draw_newton()) {
        painter.setPen(pen_newton);
        draw_graph(painter, show_a, show_b, [this](double t) { return newton_value(t); });
    }
    if (draw_hermite()) {
        painter.setPen(pen_hermite);
        draw_graph(painter, show_a, show_b, [this](double t) { return hermite_value(t); });
    }
    if (draw_parabolic()) {
        painter.setPen(pen_parabolic);
        draw_graph(painter, show_a, show_b, [this](double t) { return parabolic_value(t); });
    }
    if (draw_newton_error()) {
        painter.setPen(pen_newton);
        draw_graph(painter, show_a, show_b, [this](double t) { return newton_error(t); });
    }
    if (draw_hermite_error()) {
        painter.setPen(pen_hermite);
        draw_graph(painter, show_a, show_b, [this](double t) { return hermite_error(t); });
    }
    if (draw_parabolic_error()) {
        painter.setPen(pen_parabolic);
        draw_graph(painter, show_a, show_b, [this](double t) { return parabolic_error(t); });
    }

    /* Подписи концов отрезка и границ по вертикали */
    painter.setPen(pen_axis);
    std::snprintf(buf, sizeof(buf), "%.4g", show_a);
    painter.drawText(QPointF(4, height() - 4), buf);
    std::snprintf(buf, sizeof(buf), "%.4g", show_b);
    painter.drawText(QPointF(width() - 60, height() - 4), buf);
    std::snprintf(buf, sizeof(buf), "%.4g", max_y);
    painter.drawText(QPointF(4, 14), buf);
    std::snprintf(buf, sizeof(buf), "%.4g", min_y);
    painter.drawText(QPointF(4, height() - 20), buf);

    /* Информационная панель */
    int line = 0;
    int left_margin = width() / 2 - 180;
    if (left_margin < 70)
        left_margin = 70;

    painter.setPen(pen_function);
    painter.drawText(QPointF(left_margin, 16 + 16 * line++),
                     QString(get_function_description(k)) + "  (красный)");

    painter.setPen(pen_axis);
    std::snprintf(buf, sizeof(buf), "n = %d   s = %d   p = %d   max{|F|} = %.3e",
                  n, s, p, max_abs_F);
    painter.drawText(QPointF(left_margin, 16 + 16 * line++), buf);
    painter.drawText(QPointF(left_margin, 16 + 16 * line++),
                     QString("режим: ") + mode_description(mode));

    if (mode == 1 || mode == 4 || mode == 5 || mode == 8) {
        painter.setPen(pen_newton);
        painter.drawText(QPointF(left_margin, 16 + 16 * line++),
                         newton_ready
                             ? "метод 1: многочлен Ньютона (синий)"
                             : "метод 1: не строится (n > 50)");
    }
    if (mode == 2 || mode == 4 || mode == 6 || mode == 8) {
        painter.setPen(pen_hermite);
        painter.drawText(QPointF(left_margin, 16 + 16 * line++),
                         "метод 2: сплайн Эрмита, естеств. условия (зелёный)");
    }
    if (mode == 3 || mode == 4 || mode == 7 || mode == 8) {
        painter.setPen(pen_parabolic);
        painter.drawText(QPointF(left_margin, 16 + 16 * line++),
                         "метод 3: параболический сплайн, экстраполяция (сиреневый)");
    }
}
