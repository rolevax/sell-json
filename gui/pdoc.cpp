#include "sell/gui/pdoc.h"
#include <Qt>
#include <QKeySequence>
#include <QChar>

#include <QDebug>

PDoc::PDoc(QObject *parent) :
    QObject(parent),
    doc(new Doc)
{
    doc->registerObserver(this);
}

void PDoc::load()
{
    doc->load();
}

void PDoc::keyboard(QString key, int modifier)
{
    qDebug() << "PDoc: key:" << key << " mod:" << modifier;
    if (key.size() > 0)
        doc->keyboard(key.at(0).toLatin1());
}

int PDoc::output()
{
    return 725;
}

void PDoc::attachPRawRows(PRawRows *p)
{
    doc->registerTokensObserver(p);
}

void PDoc::observeMenu(const char *text)
{
    emit menu(text);
}

void PDoc::observePush(const char *name)
{
    emit pushed(name);
}

void PDoc::observePop()
{
    emit popped();
}

