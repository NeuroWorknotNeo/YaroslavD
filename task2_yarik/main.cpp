#include <QAction>
#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>

#include <cstdio>

#include "window.h"

int main(int argc, char **argv) {
    double a, b;
    int n, k;

    /* Все начальные параметры задаются аргументами командной строки */
    if (!(argc == 5 && std::sscanf(argv[1], "%lf", &a) == 1 &&
          std::sscanf(argv[2], "%lf", &b) == 1 && a < b &&
          std::sscanf(argv[3], "%d", &n) == 1 && n > 0 &&
          std::sscanf(argv[4], "%d", &k) == 1 && k >= 0 && k <= 6)) {
        std::printf("Запуск: %s (double a) (double b > a) (int n > 0) (int k из [0; 6])\n",
                    argv[0]);
        return 1;
    }

    QApplication app(argc, argv);

    QMainWindow *main_window = new QMainWindow;
    QMenuBar *tool_bar = new QMenuBar(main_window);
    Window *graph_area = new Window(main_window, a, b, n, k);
    QStatusBar *status_bar = new QStatusBar(main_window);

    QLabel *function_description = new QLabel;
    QLabel *number_of_points = new QLabel;
    QLabel *scale = new QLabel;
    QLabel *distortion = new QLabel;
    QLabel *max_abs_F = new QLabel;

    QAction *action;

    action = tool_bar->addAction("Сменить функцию", graph_area,
                                 SLOT(change_function()));
    action->setShortcut(QString("0"));

    action = tool_bar->addAction("Сменить набор графиков", graph_area,
                                 SLOT(change_graph()));
    action->setShortcut(QString("1"));

    action = tool_bar->addAction("Увеличить масштаб", graph_area,
                                 SLOT(increase_scale()));
    action->setShortcut(QString("2"));

    action = tool_bar->addAction("Уменьшить масштаб", graph_area,
                                 SLOT(decrease_scale()));
    action->setShortcut(QString("3"));

    action = tool_bar->addAction("Удвоить n", graph_area, SLOT(increase_points()));
    action->setShortcut(QString("4"));

    action = tool_bar->addAction("Уменьшить n вдвое", graph_area,
                                 SLOT(decrease_points()));
    action->setShortcut(QString("5"));

    action = tool_bar->addAction("Увеличить возмущение", graph_area,
                                 SLOT(add_distortion()));
    action->setShortcut(QString("6"));

    action = tool_bar->addAction("Уменьшить возмущение", graph_area,
                                 SLOT(subtract_distortion()));
    action->setShortcut(QString("7"));

    action = tool_bar->addAction("Выход", main_window, SLOT(close()));
    action->setShortcut(QString("Ctrl+Q"));

    status_bar->insertPermanentWidget(0, function_description, 3);
    QObject::connect(graph_area, SIGNAL(function_description_changed(const QString &)),
                     function_description, SLOT(setText(const QString &)));

    status_bar->insertPermanentWidget(1, number_of_points, 1);
    QObject::connect(graph_area, SIGNAL(number_of_points_changed(const QString &)),
                     number_of_points, SLOT(setText(const QString &)));

    status_bar->insertPermanentWidget(2, scale, 1);
    QObject::connect(graph_area, SIGNAL(scale_changed(const QString &)), scale,
                     SLOT(setText(const QString &)));

    status_bar->insertPermanentWidget(3, distortion, 1);
    QObject::connect(graph_area, SIGNAL(distortion_changed(const QString &)),
                     distortion, SLOT(setText(const QString &)));

    status_bar->insertPermanentWidget(4, max_abs_F, 1);
    QObject::connect(graph_area, SIGNAL(max_abs_F_changed(const QString &)),
                     max_abs_F, SLOT(setText(const QString &)));

    graph_area->init_status_bar();

    main_window->setMenuBar(tool_bar);
    main_window->setStatusBar(status_bar);
    main_window->setCentralWidget(graph_area);
    main_window->setWindowTitle("Приближение функций одной переменной");
    main_window->resize(1000, 700);
    main_window->show();

    int code = app.exec();
    delete main_window;
    return code;
}
