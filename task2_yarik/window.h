#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <memory>

#include "functions.h"

class QPainter;

/* Виджет, в котором рисуются графики функции, двух-трёх приближений
 * и их погрешностей. Реагирует на клавиши 0..7 (см. main.cpp). */
class Window : public QWidget {
    Q_OBJECT

private:
    double a = -1.;      /* левый конец исходного отрезка */
    double b = 1.;       /* правый конец исходного отрезка */
    double show_a = -1.; /* левый конец отображаемого отрезка */
    double show_b = 1.;  /* правый конец отображаемого отрезка */

    double min_y = -1.;     /* минимум отображаемых графиков */
    double max_y = 1.;      /* максимум отображаемых графиков */
    double max_abs_f = 0.;  /* max|f| на [a, b] -- для моделирования погрешности */
    double max_abs_F = 0.;  /* max|F| по отображаемым графикам на [show_a, show_b] */

    std::unique_ptr<double[]> x; /* узлы интерполяции, длина n */
    std::unique_ptr<double[]> f; /* значения функции в узлах, длина n */

    std::unique_ptr<double[]> newton_c;    /* коэффициенты многочлена Ньютона */
    std::unique_ptr<double[]> hermite_c;   /* коэффициенты сплайна Эрмита */
    std::unique_ptr<double[]> parabolic_c; /* коэффициенты параболического сплайна */

    bool newton_ready = false;
    bool hermite_ready = false;
    bool parabolic_ready = false;

    func_t function = nullptr; /* указатель на приближаемую функцию */

    int n = 10;   /* число точек интерполяции */
    int k = 0;    /* номер приближаемой функции */
    int s = 0;    /* текущий масштаб по оси X */
    int p = 0;    /* величина возмущения значения в средней точке */
    int mode = 4; /* режим отображения графиков */

    /* Режимы:
     * 0 -- f
     * 1 -- f и многочлен Ньютона
     * 2 -- f и сплайн Эрмита
     * 3 -- f и параболический сплайн
     * 4 -- f и все три приближения
     * 5 -- погрешность метода Ньютона
     * 6 -- погрешность метода Эрмита
     * 7 -- погрешность параболического сплайна
     * 8 -- погрешности всех трёх методов */
    static const int TOTAL_MODES = 9;

    /* Многочленная аппроксимация при n > 50 не строится */
    static const int MAX_NEWTON_POINTS = 50;
    /* Ограничение сверху, чтобы не исчерпать память при нажатиях клавиши 4 */
    static const int MAX_POINTS = 20000000;

public:
    Window(QWidget *parent, double a, double b, int n, int k);

    QSize minimumSizeHint() const override {
        return QSize(320, 240);
    }
    QSize sizeHint() const override {
        return QSize(1000, 700);
    }

    void init_status_bar();

public slots:
    void change_function();      /* 0 */
    void change_graph();         /* 1 */
    void increase_scale();       /* 2 */
    void decrease_scale();       /* 3 */
    void increase_points();      /* 4 */
    void decrease_points();      /* 5 */
    void add_distortion();       /* 6 */
    void subtract_distortion();  /* 7 */

signals:
    void function_description_changed(const QString &);
    void number_of_points_changed(const QString &);
    void scale_changed(const QString &);
    void distortion_changed(const QString &);
    void max_abs_F_changed(const QString &);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void allocate_arrays();
    void calculate_points();
    void build_approximations();
    void calculate_max_abs_f();
    void calculate_min_max();
    void rebuild();

    double newton_value(double t) const;
    double hermite_value(double t) const;
    double parabolic_value(double t) const;
    double newton_error(double t) const;
    double hermite_error(double t) const;
    double parabolic_error(double t) const;

    bool draw_function() const { return mode <= 4; }
    bool draw_newton() const { return (mode == 1 || mode == 4) && newton_ready; }
    bool draw_hermite() const { return (mode == 2 || mode == 4) && hermite_ready; }
    bool draw_parabolic() const { return (mode == 3 || mode == 4) && parabolic_ready; }
    bool draw_newton_error() const { return (mode == 5 || mode == 8) && newton_ready; }
    bool draw_hermite_error() const { return (mode == 6 || mode == 8) && hermite_ready; }
    bool draw_parabolic_error() const { return (mode == 7 || mode == 8) && parabolic_ready; }

    QPointF l2g(double x_loc, double y_loc) const;

    /* Подпрограмма рисования графика: получает отрезок [left, right] и
     * функцию (объект-функцию), вычисляющую значение в точке */
    template <typename Function>
    void draw_graph(QPainter &painter, double left, double right, Function func);
};

#endif /* WINDOW_H */
