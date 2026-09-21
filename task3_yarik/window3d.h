#ifndef WINDOW3D_H
#define WINDOW3D_H

#include <QWidget>
#include <vector>

#include "functions2d.h"

class QPainter;

/* Виджет, рисующий поверхность z = F(x,y) в трёхмерной проекции.
 * F -- это либо сама функция f, либо интерполяционный многочлен Ньютона P,
 * либо погрешность f - P (переключается клавишей 1). */
class Window3D : public QWidget {
    Q_OBJECT

private:
    /* Область, на которой строится интерполяция */
    double ax = -1., bx = 1., ay = -1., by = 1.;
    /* Область, которая сейчас отображается (меняется клавишами 2 и 3) */
    double sax = -1., sbx = 1., say = -1., sby = 1.;

    int nx = 10;  /* число узлов интерполяции по x */
    int ny = 10;  /* число узлов интерполяции по y */
    int mx = 40;  /* число точек сетки рисования по x */
    int my = 40;  /* число точек сетки рисования по y */
    int k = 0;    /* номер приближаемой функции */
    int s = 0;    /* текущий масштаб */
    int p = 0;    /* текущее возмущение значения в центральном узле */
    int mode = 0; /* 0 -- функция, 1 -- приближение, 2 -- погрешность */

    static const int MODES_COUNT = 3;
    static const int MAX_NODES = 64;
    static const int MAX_MESH = 128;

    double phi = 0.9;   /* угол поворота камеры вокруг оси Z */
    double theta = 0.5; /* угол подъёма камеры над плоскостью XY */

    func2_t function = nullptr;

    std::vector<double> x_nodes;  /* узлы по x, длина nx (порядок Лежа) */
    std::vector<double> y_nodes;  /* узлы по y, длина ny */
    std::vector<double> f_values; /* значения f в узлах, длина nx*ny */
    std::vector<double> coeffs;   /* коэффициенты c_ij, длина nx*ny */
    bool ready = false;           /* построен ли многочлен */

    std::vector<double> mesh_z; /* значения рисуемой функции, длина mx*my */

    double max_abs_f = 0.; /* max|f| на области -- для моделирования погрешности */
    double min_z = 0.;     /* минимум рисуемой поверхности */
    double max_z = 0.;     /* максимум рисуемой поверхности */
    double max_abs_F = 0.; /* max{|min_z|, |max_z|} */

public:
    Window3D(QWidget *parent, double ax, double bx, double ay, double by,
             int nx, int ny, int mx, int my, int k);

    QSize minimumSizeHint() const override {
        return QSize(400, 300);
    }
    QSize sizeHint() const override {
        return QSize(1000, 700);
    }

    void init_status_bar();

public slots:
    void change_function();     /* 0 */
    void change_mode();         /* 1 */
    void increase_scale();      /* 2 */
    void decrease_scale();      /* 3 */
    void increase_points();     /* 4 */
    void decrease_points();     /* 5 */
    void add_distortion();      /* 6 */
    void subtract_distortion(); /* 7 */
    void rotate_left();         /* 8 */
    void rotate_right();        /* 9 */
    void increase_mesh();       /* + */
    void decrease_mesh();       /* - */
    void camera_up();           /* Up */
    void camera_down();         /* Down */

signals:
    void function_description_changed(const QString &);
    void nodes_changed(const QString &);
    void mesh_changed(const QString &);
    void scale_changed(const QString &);
    void distortion_changed(const QString &);
    void max_abs_F_changed(const QString &);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void calculate_max_abs_f();
    void build_interpolation(); /* узлы, значения, коэффициенты c_ij */
    void build_mesh();          /* значения рисуемой поверхности на сетке */
    void update_status();

    double surface_value(double x, double y) const;

    /* Проекция точки (X, Y, Z) на экран; в depth возвращается "глубина"
     * (чем больше, тем ближе к наблюдателю) */
    QPointF project(double X, double Y, double Z, double *depth) const;
    QColor height_color(double z) const;
    void draw_box(QPainter &painter) const;
    void draw_surface(QPainter &painter) const;
    void draw_info(QPainter &painter) const;
};

#endif /* WINDOW3D_H */
