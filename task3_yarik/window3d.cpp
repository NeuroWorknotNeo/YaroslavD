#include <QPaintEvent>
#include <QPainter>
#include <QPolygonF>
#include <QString>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <limits>

#include "newton2d.h"
#include "window3d.h"

static const char *mode_description(int mode) {
    switch (mode) {
    case 1:  return "интерполяционный многочлен Ньютона P(x,y)";
    case 2:  return "погрешность f(x,y) - P(x,y)";
    default: return "исходная функция f(x,y)";
    }
}

Window3D::Window3D(QWidget *parent, double ax_in, double bx_in, double ay_in,
                   double by_in, int nx_in, int ny_in, int mx_in, int my_in,
                   int k_in)
    : QWidget(parent), ax(ax_in), bx(bx_in), ay(ay_in), by(by_in), sax(ax_in),
      sbx(bx_in), say(ay_in), sby(by_in), nx(nx_in), ny(ny_in), mx(mx_in),
      my(my_in), k(k_in) {
    function = get_function2d(k);
    calculate_max_abs_f();
    build_interpolation();
    build_mesh();
}

/* ---------- подготовка данных ---------- */

void Window3D::calculate_max_abs_f() {
    int points = 200;
    double value;

    max_abs_f = 0.;
    for (int i = 0; i <= points; i++) {
        for (int j = 0; j <= points; j++) {
            value = std::fabs(function(ax + i * (bx - ax) / points,
                                       ay + j * (by - ay) / points));
            if (value > max_abs_f)
                max_abs_f = value;
        }
    }
}

void Window3D::build_interpolation() {
    std::clock_t start = std::clock();

    x_nodes.assign(nx, 0.);
    y_nodes.assign(ny, 0.);
    f_values.assign(static_cast<size_t>(nx) * ny, 0.);
    coeffs.assign(static_cast<size_t>(nx) * ny, 0.);

    /* Узлы Чебышёва, переставленные в порядок Лежа (так форма Ньютона
     * остаётся устойчивой и при больших nx, ny) */
    make_chebyshev_nodes(nx, ax, bx, x_nodes.data());
    make_chebyshev_nodes(ny, ay, by, y_nodes.data());
    reorder_nodes_leja(nx, x_nodes.data());
    reorder_nodes_leja(ny, y_nodes.data());

    for (int i = 0; i < nx; i++)
        for (int j = 0; j < ny; j++)
            f_values[i * ny + j] = function(x_nodes[i], y_nodes[j]);

    /* Моделирование погрешности измерения в одном узле сетки */
    if (p != 0)
        f_values[(nx / 2) * ny + (ny / 2)] += p * 0.1 * max_abs_f;

    ready = (make_newton2d_coefficients(nx, ny, x_nodes.data(), y_nodes.data(),
                                        f_values.data(), coeffs.data()) == 0) &&
            !std::isnan(coeffs[0]);

    std::printf("nx = %d, ny = %d: построение коэффициентов заняло %.3f с\n", nx,
                ny, static_cast<double>(std::clock() - start) / CLOCKS_PER_SEC);
}

double Window3D::surface_value(double x, double y) const {
    if (mode == 0)
        return function(x, y);
    if (!ready)
        return std::numeric_limits<double>::quiet_NaN();
    if (mode == 1)
        return calculate_newton2d_value(x, y, nx, ny, x_nodes.data(),
                                        y_nodes.data(), coeffs.data());
    return calculate_newton2d_discrepancy(x, y, function, nx, ny,
                                          x_nodes.data(), y_nodes.data(),
                                          coeffs.data());
}

void Window3D::build_mesh() {
    std::clock_t start = std::clock();
    double value;
    bool empty = true;

    mesh_z.assign(static_cast<size_t>(mx) * my, 0.);
    min_z = 0.;
    max_z = 0.;

    for (int i = 0; i < mx; i++) {
        double x = sax + i * (sbx - sax) / (mx - 1);
        for (int j = 0; j < my; j++) {
            double y = say + j * (sby - say) / (my - 1);
            value = surface_value(x, y);
            mesh_z[i * my + j] = value;
            if (std::isnan(value) || std::isinf(value))
                continue;
            if (empty) {
                min_z = max_z = value;
                empty = false;
            } else {
                min_z = std::min(min_z, value);
                max_z = std::max(max_z, value);
            }
        }
    }

    if (empty) {
        min_z = -1.;
        max_z = 1.;
    }

    max_abs_F = std::fmax(std::fabs(min_z), std::fabs(max_z));

    std::printf("max{|F|} = %.3e   (режим: %s, nx = %d, ny = %d, mx = %d, "
                "my = %d, s = %d, p = %d, сетка построена за %.3f с)\n",
                max_abs_F, mode_description(mode), nx, ny, mx, my, s, p,
                static_cast<double>(std::clock() - start) / CLOCKS_PER_SEC);
    std::fflush(stdout);

    char buf[64];
    std::snprintf(buf, sizeof(buf), "max{|F|} = %.3e", max_abs_F);
    max_abs_F_changed(QString(buf));
}

void Window3D::update_status() {
    char buf[64];

    function_description_changed(QString(get_function2d_description(k)));
    std::snprintf(buf, sizeof(buf), "nx = %d, ny = %d", nx, ny);
    nodes_changed(QString(buf));
    std::snprintf(buf, sizeof(buf), "mx = %d, my = %d", mx, my);
    mesh_changed(QString(buf));
    std::snprintf(buf, sizeof(buf), "s = %d", s);
    scale_changed(QString(buf));
    std::snprintf(buf, sizeof(buf), "p = %d", p);
    distortion_changed(QString(buf));
    std::snprintf(buf, sizeof(buf), "max{|F|} = %.3e", max_abs_F);
    max_abs_F_changed(QString(buf));
}

void Window3D::init_status_bar() {
    update_status();
}

/* ---------- обработка клавиш ---------- */

void Window3D::change_function() {
    k = (k + 1) % FUNCTIONS2D_COUNT;
    function = get_function2d(k);
    p = 0;

    calculate_max_abs_f();
    build_interpolation();
    build_mesh();
    update_status();
    update();
}

void Window3D::change_mode() {
    mode = (mode + 1) % MODES_COUNT;
    build_mesh();
    update_status();
    update();
}

void Window3D::increase_scale() {
    double cx = 0.5 * (sax + sbx), cy = 0.5 * (say + sby);

    s++;
    sax = cx + 0.5 * (sax - cx);
    sbx = cx + 0.5 * (sbx - cx);
    say = cy + 0.5 * (say - cy);
    sby = cy + 0.5 * (sby - cy);

    build_mesh();
    update_status();
    update();
}

void Window3D::decrease_scale() {
    double cx = 0.5 * (sax + sbx), cy = 0.5 * (say + sby);

    s--;
    sax = cx + 2. * (sax - cx);
    sbx = cx + 2. * (sbx - cx);
    say = cy + 2. * (say - cy);
    sby = cy + 2. * (sby - cy);

    build_mesh();
    update_status();
    update();
}

void Window3D::increase_points() {
    if (2 * nx > MAX_NODES || 2 * ny > MAX_NODES) {
        std::printf("Достигнут предел числа узлов интерполяции (%d)\n", MAX_NODES);
        return;
    }
    nx *= 2;
    ny *= 2;

    build_interpolation();
    build_mesh();
    update_status();
    update();
}

void Window3D::decrease_points() {
    if (nx < 4 || ny < 4)
        return;
    nx /= 2;
    ny /= 2;

    build_interpolation();
    build_mesh();
    update_status();
    update();
}

void Window3D::add_distortion() {
    p++;
    build_interpolation();
    build_mesh();
    update_status();
    update();
}

void Window3D::subtract_distortion() {
    p--;
    build_interpolation();
    build_mesh();
    update_status();
    update();
}

void Window3D::rotate_left() {
    phi -= M_PI / 12.;
    update();
}

void Window3D::rotate_right() {
    phi += M_PI / 12.;
    update();
}

void Window3D::camera_up() {
    theta += M_PI / 24.;
    if (theta > 1.5)
        theta = 1.5;
    update();
}

void Window3D::camera_down() {
    theta -= M_PI / 24.;
    if (theta < -1.5)
        theta = -1.5;
    update();
}

void Window3D::increase_mesh() {
    if (2 * mx > MAX_MESH || 2 * my > MAX_MESH) {
        std::printf("Достигнут предел сетки рисования (%d)\n", MAX_MESH);
        return;
    }
    mx *= 2;
    my *= 2;

    build_mesh();
    update_status();
    update();
}

void Window3D::decrease_mesh() {
    if (mx < 8 || my < 8)
        return;
    mx /= 2;
    my /= 2;

    build_mesh();
    update_status();
    update();
}

/* ---------- рисование ---------- */

QPointF Window3D::project(double X, double Y, double Z, double *depth) const {
    double hx = 0.5 * (sbx - sax), hy = 0.5 * (sby - say);
    double cx = 0.5 * (sax + sbx), cy = 0.5 * (say + sby);
    double dz = max_z - min_z;
    double nX, nY, nZ;
    double sx, sy, scale;

    nX = (hx > 0. ? (X - cx) / hx : 0.);
    nY = (hy > 0. ? (Y - cy) / hy : 0.);
    nZ = (dz > std::numeric_limits<double>::epsilon()
              ? 0.8 * (2. * (Z - min_z) / dz - 1.)
              : 0.);

    /* Орты экрана для камеры, заданной углами phi (поворот) и theta (подъём) */
    sx = -nX * std::sin(phi) + nY * std::cos(phi);
    sy = -(nX * std::cos(phi) + nY * std::sin(phi)) * std::sin(theta) +
         nZ * std::cos(theta);
    if (depth)
        *depth = (nX * std::cos(phi) + nY * std::sin(phi)) * std::cos(theta) +
                 nZ * std::sin(theta);

    scale = 0.32 * std::min(width(), height());
    return QPointF(0.5 * width() + scale * sx, 0.55 * height() - scale * sy);
}

QColor Window3D::height_color(double z) const {
    double t = (max_z - min_z > std::numeric_limits<double>::epsilon()
                    ? (z - min_z) / (max_z - min_z)
                    : 0.5);
    int r, g, b;

    if (t < 0.)
        t = 0.;
    if (t > 1.)
        t = 1.;

    /* синий -> голубой -> зелёный -> жёлтый -> красный */
    if (t < 0.25) {
        double u = t / 0.25;
        r = 0;
        g = static_cast<int>(255 * u);
        b = 255;
    } else if (t < 0.5) {
        double u = (t - 0.25) / 0.25;
        r = 0;
        g = 255;
        b = static_cast<int>(255 * (1. - u));
    } else if (t < 0.75) {
        double u = (t - 0.5) / 0.25;
        r = static_cast<int>(255 * u);
        g = 255;
        b = 0;
    } else {
        double u = (t - 0.75) / 0.25;
        r = 255;
        g = static_cast<int>(255 * (1. - u));
        b = 0;
    }

    return QColor(r, g, b);
}

void Window3D::draw_box(QPainter &painter) const {
    double corner_x[4] = {sax, sbx, sbx, sax};
    double corner_y[4] = {say, say, sby, sby};
    QPen pen(QColor(150, 150, 150), 1);

    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    for (int i = 0; i < 4; i++) {
        int j = (i + 1) % 4;
        painter.drawLine(project(corner_x[i], corner_y[i], min_z, nullptr),
                         project(corner_x[j], corner_y[j], min_z, nullptr));
        painter.drawLine(project(corner_x[i], corner_y[i], max_z, nullptr),
                         project(corner_x[j], corner_y[j], max_z, nullptr));
        painter.drawLine(project(corner_x[i], corner_y[i], min_z, nullptr),
                         project(corner_x[i], corner_y[i], max_z, nullptr));
    }

    /* Подписи осей у одного из углов основания */
    painter.setPen(QPen(Qt::black, 1));
    painter.drawText(project(sbx, say, min_z, nullptr), QString("x = %1").arg(sbx));
    painter.drawText(project(sax, sby, min_z, nullptr), QString("y = %1").arg(sby));
    painter.drawText(project(sax, say, max_z, nullptr), QString("z = %1").arg(max_z));
    painter.drawText(project(sax, say, min_z, nullptr), QString("z = %1").arg(min_z));
}

void Window3D::draw_surface(QPainter &painter) const {
    int cells = (mx - 1) * (my - 1);
    std::vector<std::pair<double, int>> order;
    bool draw_edges = (mx <= 64 && my <= 64);

    if (cells <= 0)
        return;

    order.reserve(cells);
    for (int i = 0; i < mx - 1; i++) {
        for (int j = 0; j < my - 1; j++) {
            double z00 = mesh_z[i * my + j];
            double z10 = mesh_z[(i + 1) * my + j];
            double z01 = mesh_z[i * my + j + 1];
            double z11 = mesh_z[(i + 1) * my + j + 1];
            double depth, sum = 0.;
            double x_mid, y_mid;

            if (std::isnan(z00) || std::isnan(z10) || std::isnan(z01) ||
                std::isnan(z11))
                continue;

            x_mid = sax + (i + 0.5) * (sbx - sax) / (mx - 1);
            y_mid = say + (j + 0.5) * (sby - say) / (my - 1);
            sum = 0.25 * (z00 + z10 + z01 + z11);
            project(x_mid, y_mid, sum, &depth);
            order.push_back(std::make_pair(depth, i * (my - 1) + j));
        }
    }

    /* Алгоритм художника: дальние клетки рисуются первыми */
    std::sort(order.begin(), order.end());

    for (size_t idx = 0; idx < order.size(); idx++) {
        int cell = order[idx].second;
        int i = cell / (my - 1);
        int j = cell % (my - 1);

        double x0 = sax + i * (sbx - sax) / (mx - 1);
        double x1 = sax + (i + 1) * (sbx - sax) / (mx - 1);
        double y0 = say + j * (sby - say) / (my - 1);
        double y1 = say + (j + 1) * (sby - say) / (my - 1);

        double z00 = mesh_z[i * my + j];
        double z10 = mesh_z[(i + 1) * my + j];
        double z11 = mesh_z[(i + 1) * my + j + 1];
        double z01 = mesh_z[i * my + j + 1];

        QPolygonF quad;
        quad << project(x0, y0, z00, nullptr) << project(x1, y0, z10, nullptr)
             << project(x1, y1, z11, nullptr) << project(x0, y1, z01, nullptr);

        QColor color = height_color(0.25 * (z00 + z10 + z01 + z11));
        painter.setBrush(color);
        if (draw_edges)
            painter.setPen(QPen(color.darker(160), 1));
        else
            painter.setPen(QPen(color, 1));
        painter.drawPolygon(quad);
    }
}

void Window3D::draw_info(QPainter &painter) const {
    char buf[256];
    int line = 0;

    painter.setPen(QPen(Qt::black, 1));
    painter.drawText(QPointF(8, 16 + 16 * line++),
                     QString(get_function2d_description(k)));
    painter.drawText(QPointF(8, 16 + 16 * line++),
                     QString("режим: ") + mode_description(mode));
    std::snprintf(buf, sizeof(buf), "узлы: nx = %d, ny = %d   сетка: mx = %d, my = %d",
                  nx, ny, mx, my);
    painter.drawText(QPointF(8, 16 + 16 * line++), buf);
    std::snprintf(buf, sizeof(buf), "s = %d   p = %d   max{|F|} = %.3e", s, p,
                  max_abs_F);
    painter.drawText(QPointF(8, 16 + 16 * line++), buf);
    std::snprintf(buf, sizeof(buf),
                  "область: x in [%.3g, %.3g], y in [%.3g, %.3g]", sax, sbx, say,
                  sby);
    painter.drawText(QPointF(8, 16 + 16 * line++), buf);
    std::snprintf(buf, sizeof(buf), "камера: поворот %.0f°, подъём %.0f°",
                  phi * 180. / M_PI, theta * 180. / M_PI);
    painter.drawText(QPointF(8, 16 + 16 * line++), buf);
    if (!ready)
        painter.drawText(QPointF(8, 16 + 16 * line++),
                         "многочлен не построен (проверьте число узлов)");
}

void Window3D::paintEvent(QPaintEvent *event) {
    (void)event;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.fillRect(rect(), Qt::white);

    draw_box(painter);
    draw_surface(painter);
    draw_info(painter);
}
