#include "gui/prawrows.h"

#include "QDebug"

PRawRows::PRawRows(QObject *parent) :
    QObject(parent)
{

}

void PRawRows::observeInsert(const Token &token)
{
    emit inserted(createTokenVar(token));
}

void PRawRows::observeCoord(size_t r, size_t c)
{
    emit coorded(r, c);
}

void PRawRows::observeLight(size_t lbr, size_t lbc,
                            size_t ler, size_t lec,
                            size_t hbr, size_t hbc,
                            size_t her, size_t hec)
{
    emit lighted(lbr, lbc, ler, lec, hbr, hbc, her, hec);
}

QVariant PRawRows::createTokenVar(const Token &token)
{
    QVariantMap map;
    map.insert("modelText", QString::fromStdString(token.getText()));
    return QVariant::fromValue(map);
}

