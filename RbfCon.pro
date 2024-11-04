QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CXXFLAGS_RELEASE += -O4 -fopenmp -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native  -ffast-math -fopt-info -Ofast
QMAKE_CFLAGS_RELEASE += -O4 -fopenmp -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -ffast-math -Ofast

QMAKE_CXXFLAGS_RELEASE += -std=c++11
QMAKE_CFLAGS_RELEASE += -std=c++11

QMAKE_CXXFLAGS += -O4 -fopenmp -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -ffast-math -Ofast
QMAKE_CFLAGS += -O4 -fopenmp -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native  -ffast-math -Ofast

SOURCES += \
        CORE/parameter.cpp \
        CORE/parameterlist.cpp \
        GE/converter.cc \
        GE/cprogram.cc \
        GE/doublestack.cc \
        GE/fparser.cc \
        GE/integeranneal.cpp \
        GE/neuralparser.cc \
        GE/neuralprogram.cc \
        GE/nncneuralprogram.cc \
        GE/population.cc \
        GE/program.cc \
        GE/rule.cc \
        GE/sigprogram.cc \
        GE/symbol.cc \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    CORE/parameter.h \
    CORE/parameterlist.h \
    GE/converter.h \
    GE/cprogram.h \
    GE/doublestack.h \
    GE/fparser.hh \
    GE/integeranneal.h \
    GE/neuralparser.h \
    GE/neuralprogram.h \
    GE/nncneuralprogram.h \
    GE/population.h \
    GE/program.h \
    GE/rule.h \
    GE/sigprogram.h \
    GE/symbol.h
