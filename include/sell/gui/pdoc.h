#ifndef PDOC_H
#define PDOC_H

#include <QObject>
#include <memory>
#include "sell/core/doc.h"
#include "sell/gui/prawrows.h"

class PDoc : public QObject
{
    Q_OBJECT
public:
    explicit PDoc(QObject *parent = 0);
    Q_INVOKABLE void load();
    Q_INVOKABLE void keyboard(QString key, int modifier);
    Q_INVOKABLE int output();
    Q_INVOKABLE void attachPRawRows(PRawRows *p);

    void observeTension(bool b);
    void observePush(const char *name);
    void observePop();

signals:
    void tension(bool b);
    void pushed(QString name);
    void popped();

public slots:

private:
    std::unique_ptr<Doc> doc;
};

#endif // PDOC_H
