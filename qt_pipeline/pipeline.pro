TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread

SOURCES += \
        barrier.cpp \
        ex_thread.cpp \
        id_thread.cpp \
        if_thread.cpp \
        main.cpp \
        master_thread.cpp \
        mem_thread.cpp \
        wb_thread.cpp

HEADERS += \
    barrier.h \
    ex_thread.h \
    id_thread.h \
    if_thread.h \
    master_thread.h \
    mem_thread.h \
    wb_thread.h

DISTFILES += \
    ../build-pipeline-Desktop_Qt_5_13_1_GCC_64bit-Debug/HilillosPruebaFinal/0.txt \
    ../build-pipeline-Desktop_Qt_5_13_1_GCC_64bit-Debug/HilillosPruebaFinal/1.txt \
    ../build-pipeline-Desktop_Qt_5_13_1_GCC_64bit-Debug/HilillosPruebaFinal/2.txt \
    HilillosPruebaFinal/1.txt \
    HilillosPruebaFinal/2.txt \
    HilillosPruebaFinal/2019-2-CI0120-HilillosPruebaFinal-MEM y Regs.pdf \
    HilillosPruebaFinal/2019-2-CI0120-HilillosPruebaFinal-MEM y Regs.xlsx \
    HilillosPruebaFinal/3.txt \
    HilillosPruebaFinal/4.txt \
    HilillosPruebaFinal/5.txt \
    HilillosPruebaFinal/6.txt
