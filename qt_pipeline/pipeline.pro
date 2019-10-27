TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread

SOURCES += \
        ex_thread.cpp \
        id_thread.cpp \
        if_thread.cpp \
        main.cpp \
        master_thread.cpp \
        mem_thread.cpp \
        wb_thread.cpp

HEADERS += \
    ex_thread.h \
    id_thread.h \
    if_thread.h \
    master_thread.h \
    mem_thread.h \
    wb_thread.h
