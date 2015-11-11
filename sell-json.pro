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
    ast/pigast.cpp \
    core/bonetoken.cpp \
    core/fleshtoken.cpp \
    core/soultoken.cpp \
    core/tokens.cpp \
    core/hammer.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

INCLUDEPATH += "include"

HEADERS += \
    include/core/doc.h \
    include/core/token.h \
    include/ast/ast.h \
    include/ast/listast.h \
    include/ast/mapast.h \
    include/ast/astconverter.h \
    include/gui/pdoc.h \
    include/gui/prawrows.h \
    include/ast/scalarast.h \
    include/ast/rootast.h \
    include/ast/pigast.h \
    include/core/bonetoken.h \
    include/core/fleshtoken.h \
    include/core/soultoken.h \
    include/core/tokens.h \
    include/core/hammer.h \
    include/core/tokensobserver.h

