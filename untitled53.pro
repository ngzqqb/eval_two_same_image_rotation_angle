QT       += core gui
QT += widgets

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/MainWindow.cpp

HEADERS += \
    $$PWD/MainWindow.hpp

FORMS += \
    $$PWD/MainWindow.ui

# out put name ...
CONFIG(debug,debug|release){
    TARGET = opencv_test_angle_debug
}else{
    TARGET = opencv_test_angle
}

#out put dir ...
DESTDIR = $$[QT_INSTALL_BINS]

CONFIG(debug,debug|release){#debug ...
    LIBS += -L$$[QT_INSTALL_LIBS] -lopencv_world420d
}else{#release ...
    LIBS += -L$$[QT_INSTALL_LIBS] -lopencv_world420
    DEFINES *= NDEBUG
}

DEFINES *= THE_DEBUG_PATH=\\\"$$PWD\\\"
