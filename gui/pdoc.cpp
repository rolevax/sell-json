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

void PDoc::load(QString filename)
{
    try {
        doc->load(filename.toStdString());
    } catch (const std::exception &e) {
        emit message(e.what());
    }
}

void PDoc::keyboard(QString key, int modifier)
{
    (void) modifier;
    if (key.size() > 0)
        doc->keyboard(key.at(0).toLatin1());
}

void PDoc::attachPRawRows(PRawRows *p)
{
    doc->registerTokensObserver(p);
}

void PDoc::observeTension(bool b)
{
    emit tension(b);
}

void PDoc::observePush(const char *name)
{
    emit pushed(name);
}

void PDoc::observePop()
{
    emit popped();
}

