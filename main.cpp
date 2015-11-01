#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

#include "gui/pdoc.h"
#include "gui/prawrows.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<PDoc>("sell", 1, 0, "PDoc");
    qmlRegisterType<PRawRows>("sell", 1, 0, "PRawRows");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
