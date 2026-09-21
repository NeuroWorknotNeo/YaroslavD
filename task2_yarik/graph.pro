QT += widgets

TARGET = graph
CONFIG += console c++17
QMAKE_CXXFLAGS += -O2 -Wall -Wextra

SOURCES += \
    main.cpp \
    functions.cpp \
    newton_interpolation.cpp \
    hermite_natural.cpp \
    parabolic_extrapolation.cpp \
    window.cpp

HEADERS += \
    functions.h \
    newton_interpolation.h \
    hermite_natural.h \
    parabolic_extrapolation.h \
    window.h
