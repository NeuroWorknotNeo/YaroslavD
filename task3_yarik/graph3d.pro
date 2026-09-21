QT += widgets

TARGET = graph3d
CONFIG += console c++17
QMAKE_CXXFLAGS += -O2 -Wall -Wextra

SOURCES += \
    main.cpp \
    functions2d.cpp \
    newton2d.cpp \
    window3d.cpp

HEADERS += \
    functions2d.h \
    newton2d.h \
    window3d.h
