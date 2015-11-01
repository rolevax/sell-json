TEMPLATE = app

QT += qml quick

CONFIG += c++11

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += main.cpp \
    core/doc.cpp \
    core/token.cpp \
    core/rawrows.cpp \
    gui/pdoc.cpp \
    gui/prawrows.cpp \
    ast/ast.cpp \
    ast/listast.cpp \
    ast/mapast.cpp \
    ast/astconverter.cpp \
    mode/mode.cpp \
    mode/viewmode.cpp \
    ast/scalarast.cpp \
    ast/rootast.cpp \
    core/rawrowswriter.cpp \
    ast/pigast.cpp \
    core/bonetoken.cpp \
    core/fleshtoken.cpp \
    core/soultoken.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

INCLUDEPATH += "include"

HEADERS += \
    include/mode/mode.h \
    include/mode/viewmode.h \
    include/core/doc.h \
    include/core/token.h \
    include/core/rawrows.h \
    include/core/rawrowsobserver.h \
    include/ast/ast.h \
    include/ast/listast.h \
    include/ast/mapast.h \
    include/ast/astconverter.h \
    include/gui/pdoc.h \
    include/gui/prawrows.h \
    include/ast/scalarast.h \
    include/ast/rootast.h \
    include/core/rawrowswriter.h \
    include/ast/pigast.h \
    include/core/bonetoken.h \
    include/core/fleshtoken.h \
    include/core/soultoken.h

