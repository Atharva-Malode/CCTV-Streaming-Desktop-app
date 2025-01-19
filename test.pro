QT       += core gui widgets
CONFIG   += c++17

TARGET   = TestApp
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/stream.cpp

HEADERS += \
    include/mainwindow.h \
    include/stream.h

FORMS += \
    ui/stream.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
