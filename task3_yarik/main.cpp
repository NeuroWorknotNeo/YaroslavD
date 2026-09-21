#include <QAction>
#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>

#include <cstdio>

#include "functions2d.h"
#include "window3d.h"

int main(int argc, char **argv) {
    /* Значения по умолчанию, если аргументы не заданы */
    double a = -1., b = 1., c = -1., d = 1.;
    int nx = 10, ny = 10, mx = 40, my = 40, k = 7;

    if (argc != 1) {
        if (!(argc == 10 && std::sscanf(argv[1], "%lf", &a) == 1 &&
              std::sscanf(argv[2], "%lf", &b) == 1 &&
              std::sscanf(argv[3], "%lf", &c) == 1 &&
              std::sscanf(argv[4], "%lf", &d) == 1 &&
              std::sscanf(argv[5], "%d", &nx) == 1 &&
              std::sscanf(argv[6], "%d", &ny) == 1 &&
              std::sscanf(argv[7], "%d", &mx) == 1 &&
              std::sscanf(argv[8], "%d", &my) == 1 &&
              std::sscanf(argv[9], "%d", &k) == 1 && a < b && c < d &&
              nx >= 2 && ny >= 2 && mx >= 2 && my >= 2 && k >= 0 &&
              k < FUNCTIONS2D_COUNT)) {
            std::printf("Запуск: %s a b c d nx ny mx my k\n"
                        "    a < b   -- границы по x (double),\n"
                        "    c < d   -- границы по y (double),\n"
                        "    nx, ny  -- число узлов интерполяции (>= 2),\n"
                        "    mx, my  -- число точек сетки рисования (>= 2),\n"
                        "    k       -- номер функции из [0; %d].\n"
                        "Без аргументов берутся значения по умолчанию.\n",
                        argv[0], FUNCTIONS2D_COUNT - 1);
            return 1;
        }
    }

    QApplication app(argc, argv);

    QMainWindow *main_window = new QMainWindow;
    QMenuBar *tool_bar = new QMenuBar(main_window);
    Window3D *graph_area = new Window3D(main_window, a, b, c, d, nx, ny, mx, my, k);
    QStatusBar *status_bar = new QStatusBar(main_window);

    QLabel *function_description = new QLabel;
    QLabel *nodes = new QLabel;
    QLabel *mesh = new QLabel;
    QLabel *scale = new QLabel;
    QLabel *distortion = new QLabel;
    QLabel *max_abs_F = new QLabel;

    QAction *action;

    action = tool_bar->addAction("Сменить функцию", graph_area,
                                 SLOT(change_function()));
    action->setShortcut(QString("0"));

    action = tool_bar->addAction("Функция / приближение / погрешность",
                                 graph_area, SLOT(change_mode()));
    action->setShortcut(QString("1"));

    action = tool_bar->addAction("Увеличить масштаб", graph_area,
                                 SLOT(increase_scale()));
    action->setShortcut(QString("2"));

    action = tool_bar->addAction("Уменьшить масштаб", graph_area,
                                 SLOT(decrease_scale()));
    action->setShortcut(QString("3"));

    action = tool_bar->addAction("Удвоить nx, ny", graph_area,
                                 SLOT(increase_points()));
    action->setShortcut(QString("4"));

    action = tool_bar->addAction("Уменьшить nx, ny вдвое", graph_area,
                                 SLOT(decrease_points()));
    action->setShortcut(QString("5"));

    action = tool_bar->addAction("Увеличить возмущение", graph_area,
                                 SLOT(add_distortion()));
    action->setShortcut(QString("6"));

    action = tool_bar->addAction("Уменьшить возмущение", graph_area,
                                 SLOT(subtract_distortion()));
    action->setShortcut(QString("7"));

    action = tool_bar->addAction("Повернуть влево", graph_area,
                                 SLOT(rotate_left()));
    action->setShortcut(QString("8"));

    action = tool_bar->addAction("Повернуть вправо", graph_area,
                                 SLOT(rotate_right()));
    action->setShortcut(QString("9"));

    QMenu *extra = tool_bar->addMenu("Ещё");

    action = extra->addAction("Удвоить сетку рисования", graph_area,
                              SLOT(increase_mesh()));
    action->setShortcut(QString("+"));

    action = extra->addAction("Уменьшить сетку рисования вдвое", graph_area,
                              SLOT(decrease_mesh()));
    action->setShortcut(QString("-"));

    action = extra->addAction("Камера выше", graph_area, SLOT(camera_up()));
    action->setShortcut(QString("Up"));

    action = extra->addAction("Камера ниже", graph_area, SLOT(camera_down()));
    action->setShortcut(QString("Down"));

    action = extra->addAction("Выход", main_window, SLOT(close()));
    action->setShortcut(QString("Ctrl+Q"));

    status_bar->insertPermanentWidget(0, function_description, 3);
    QObject::connect(graph_area, SIGNAL(function_description_changed(const QString &)),
                     function_description, SLOT(setText(const QString &)));

    status_bar->insertPermanentWidget(1, nodes, 1);
    QObject::connect(graph_area, SIGNAL(nodes_changed(const QString &)), nodes,
                     SLOT(setText(const QString &)));

    status_bar->insertPermanentWidget(2, mesh, 1);
    QObject::connect(graph_area, SIGNAL(mesh_changed(const QString &)), mesh,
                     SLOT(setText(const QString &)));

    status_bar->insertPermanentWidget(3, scale, 1);
    QObject::connect(graph_area, SIGNAL(scale_changed(const QString &)), scale,
                     SLOT(setText(const QString &)));

    status_bar->insertPermanentWidget(4, distortion, 1);
    QObject::connect(graph_area, SIGNAL(distortion_changed(const QString &)),
                     distortion, SLOT(setText(const QString &)));

    status_bar->insertPermanentWidget(5, max_abs_F, 1);
    QObject::connect(graph_area, SIGNAL(max_abs_F_changed(const QString &)),
                     max_abs_F, SLOT(setText(const QString &)));

    graph_area->init_status_bar();

    main_window->setMenuBar(tool_bar);
    main_window->setStatusBar(status_bar);
    main_window->setCentralWidget(graph_area);
    main_window->setWindowTitle("Интерполяция функции двух переменных по формуле Ньютона");
    main_window->resize(1000, 700);
    main_window->show();

    int code = app.exec();
    delete main_window;
    return code;
}
