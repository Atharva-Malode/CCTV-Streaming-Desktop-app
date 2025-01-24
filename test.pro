QT       += core gui widgets
CONFIG   += c++17

TARGET   = TestApp
TEMPLATE = app

INCLUDEPATH += "C:/Users/HP/Desktop/Atharva/opencv-qt/build/install/include"
INCLUDEPATH += "C:/Users/HP/Desktop/Atharva/opencv-qt/build/install/include/opencv2"

LIBS += -L"C:/Users/HP/Desktop/Atharva/opencv-qt/build/install/x64/mingw/bin" \
        -lopencv_core4100 \
        -lopencv_imgproc4100 \
        -lopencv_highgui4100 \
        -lopencv_imgcodecs4100


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
