TEMPLATE = app

QT += qml quick

CONFIG += c++11

CONFIG(release, debug|release): DEFINES += QT_NO_DEBUG_OUTPUT
CONFIG(release, debug|release): DEFINES += NDEBUG

SOURCES += main.cpp \
    core/doc.cpp \
    core/token.cpp \
    gui/pdoc.cpp \
    gui/prawrows.cpp \
    ast/ast.cpp \
    ast/listast.cpp \
    ast/mapast.cpp \
    ast/astconverter.cpp \
    ast/scalarast.cpp \
    ast/rootast.cpp \
    core/bonetoken.cpp \
    core/fleshtoken.cpp \
    core/soultoken.cpp \
    core/tokens.cpp \
    core/hammer.cpp \
    mode/mode.cpp \
    mode/viewmode.cpp \
    mode/menumode.cpp \
    mode/stringinputmode.cpp \
    mode/numberinputmode.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

INCLUDEPATH += "include"

HEADERS += \
    include/sell/core/doc.h \
    include/sell/core/token.h \
    include/sell/ast/ast.h \
    include/sell/ast/listast.h \
    include/sell/ast/mapast.h \
    include/sell/ast/astconverter.h \
    include/sell/gui/pdoc.h \
    include/sell/gui/prawrows.h \
    include/sell/ast/scalarast.h \
    include/sell/ast/rootast.h \
    include/sell/core/bonetoken.h \
    include/sell/core/fleshtoken.h \
    include/sell/core/soultoken.h \
    include/sell/core/tokens.h \
    include/sell/core/hammer.h \
    include/sell/core/tokensobserver.h \
    include/sell/mode/mode.h \
    include/sell/mode/viewmode.h \
    include/sell/mode/menumode.h \
    include/sell/mode/stringinputmode.h \
    include/sell/mode/numberinputmode.h

