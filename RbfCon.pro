QT -= gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
        CORE/dataset.cpp \
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
        METHODS/adam.cpp \
        METHODS/armadillo1.cpp \
        METHODS/armijosearch.cpp \
        METHODS/bfgs.cpp \
        METHODS/collection.cpp \
        METHODS/differentialevolution.cpp \
        METHODS/doublebox.cpp \
        METHODS/editlogger.cpp \
        METHODS/ego.cpp \
        METHODS/fibonaccisearch.cpp \
        METHODS/filelogger.cpp \
        METHODS/genetic.cpp \
        METHODS/goldensearch.cpp \
        METHODS/gradientdescent.cpp \
        METHODS/gwooptimizer.cpp \
        METHODS/hybridmethod.cpp \
        METHODS/ipso.cpp \
        METHODS/kmeanssampler.cpp \
        METHODS/lbfgs.cpp \
        METHODS/linesearch.cpp \
        METHODS/logger.cpp \
        METHODS/maxwellsampler.cpp \
        METHODS/mean.cpp \
        METHODS/multistart.cpp \
        METHODS/neldermead.cpp \
        METHODS/ofalgorithm.cpp \
        METHODS/optimizer.cpp \
        METHODS/problem.cpp \
        METHODS/problemsampler.cpp \
        METHODS/simanmethod.cpp \
        METHODS/similarity.cpp \
        METHODS/triangularsampler.cpp \
        METHODS/uniformsampler.cpp \
        METHODS/woa.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    CORE/dataset.h \
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
    GE/symbol.h \
    METHODS/adam.h \
    METHODS/armadillo1.h \
    METHODS/armijosearch.h \
    METHODS/bfgs.h \
    METHODS/collection.h \
    METHODS/differentialevolution.h \
    METHODS/doublebox.h \
    METHODS/editlogger.h \
    METHODS/ego.h \
    METHODS/fibonaccisearch.h \
    METHODS/filelogger.h \
    METHODS/genetic.h \
    METHODS/goldensearch.h \
    METHODS/gradientdescent.h \
    METHODS/gwooptimizer.h \
    METHODS/hybridmethod.h \
    METHODS/ipso.h \
    METHODS/kmeanssampler.h \
    METHODS/lbfgs.h \
    METHODS/linesearch.h \
    METHODS/logger.h \
    METHODS/maxwellsampler.h \
    METHODS/mean.h \
    METHODS/multistart.h \
    METHODS/neldermead.h \
    METHODS/ofalgorithm.h \
    METHODS/optimizer.h \
    METHODS/problem.h \
    METHODS/problemsampler.h \
    METHODS/simanmethod.h \
    METHODS/similarity.h \
    METHODS/triangularsampler.h \
    METHODS/uniformsampler.h \
    METHODS/woa.h
