#include "gui/pdoc.h"
#include <Qt>
#include <QKeySequence>

#include <QDebug>

PDoc::PDoc(QObject *parent) :
    QObject(parent),
    doc(new Doc)
{
}

void PDoc::load()
{
    doc->load();
}

void PDoc::keyboard(int key)
{
    qDebug() << "PDoc: key " << QKeySequence(key).toString()
             << "(" << key << ")" << " pressed. ";
    if (Qt::Key_A <= key && key <= Qt::Key_Z) {
        doc->keyboard('a' + (key - Qt::Key_A));
    } else if (Qt::Key_Space == key) {
        doc->keyboard(' ');
    } else {
        qDebug() << "PDoc: unsupported key pressed";
    }
}

int PDoc::output()
{
    return 725;
}

void PDoc::attachPRawRows(PRawRows *p)
{
    doc->registerRawRowsObserver(p);
}

