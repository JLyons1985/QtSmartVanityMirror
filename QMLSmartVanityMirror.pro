TEMPLATE = app

QT += qml quick
QT += webengine
QT += network
QT += multimedia

CONFIG += c++11

unix: {
    INCLUDEPATH += /home/jnlyons2/raspi/sysroot/usr/local/include
    LIBS += -L/home/jnlyons2/raspi/sysroot/usr/local/lib \
        -lwiringPi
    DEPENDPATH += /home/jnlyons2/raspi/sysroot/usr/local/include
}
else: {
    INCLUDEPATH += "C:\OpenCV-3.1.0\opencv\build\include"
    LIBS += -LC:\\OpenCV-3.1.0\\opencv\\build\\x64\\vc14\\lib\\ -lopencv_world310
    DEPENDPATH += "C:\OpenCV-3.1.0\opencv\build\include"
}

SOURCES += main.cpp \
    Sources/clockthread.cpp \
    Sources/DIALThread.cpp \
    Sources/MainWindowStatesThread.cpp \
    Sources/RaspiGPIOThread.cpp \
    Sources/TrafficThread.cpp \
    Sources/WeatherRetrieverThread.cpp \
    Sources/MainFormHandler.cpp \
    DataObject.cpp \
    Sources/calendaritem.cpp \
    Sources/speechaudiorecorder.cpp \
    Sources/speechdecoder.cpp \
    Sources/wxforecastitem.cpp \
    Sources/quotethread.cpp \
    Sources/texttospeech.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
target.path = /tmp/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Headers/clockthread.h \
    Headers/DIALThread.h \
    Headers/MainWindowStatesThread.h \
    Headers/RaspiGPIOThread.h \
    Headers/TrafficThread.h \
    Headers/WeatherRetrieverThread.h \
    Headers/MainFormHandler.h \
    DataObject.h \
    Headers/calendaritem.h \
    Headers/speechaudiorecorder.h \
    Headers/speechdecoder.h \
    Headers/wxforecastitem.h \
    Headers/quotethread.h \
    Headers/texttospeech.h
DISTFILES +=


