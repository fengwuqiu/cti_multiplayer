TEMPLATE = app
TARGET = cti_multiplayer
INCLUDEPATH += . /user/include
LIBS += -lzmq
DEFINES += QT_MESSAGELOGCONTEXT

QT += multimedia multimediawidgets widgets core gui
HEADERS = \
    include/mainwindow.h \
    include/common.h \
    include/kvconf.h \
    include/protocol.h \
    include/zmq_socket.h \
    include/zmq_utils.h \
    include/zmqserver.h \
    include/zmq.h
SOURCES = \
    src/main.cpp \
    src/mainwindow.cpp \ 
    src/kvconf.cpp \
    src/common.cpp \
    src/protocol.cpp \
    src/zmq_socket.cpp \
    src/zmqserver.cpp

CONFIG(release, debug|release) {
DESTDIR = $${DLLDIR}
OBJECTS_DIR = release/.obj
MOC_DIR = release/.moc
RCC_DIR = release/.rcc
UI_DIR = release/.ui
}
CONFIG(debug, debug|release) {
DESTDIR = $${DLLDIR}
OBJECTS_DIR = debug/.obj
MOC_DIR = debug/.moc
RCC_DIR = debug/.rcc
UI_DIR = debug/.ui
}
