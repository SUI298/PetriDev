QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    abstractpt.cpp \
    annotation.cpp \
    connector.cpp \
    main.cpp \
    mainwindow.cpp \
    nameobject.cpp \
    petri.cpp \
    petriview.cpp \
    place.cpp \
    transition.cpp \
    zoomobject.cpp

HEADERS += \
    abstractpt.h \
    addplacecmd.h \
    addtokencmd.h \
    addtransitioncmd.h \
    allcommands.h \
    annotation.h \
    appendtextcmd.h \
    connector.h \
    deleteitemcmd.h \
    deletetextcmd.h \
    globaltypes.h \
    mainwindow.h \
    movecmd.h \
    nameobject.h \
    petri.h \
    petriview.h \
    place.h \
    stopconnectorcmd.h \
    transition.h \
    zoomobject.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
