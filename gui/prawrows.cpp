#include "core/tokens.h"
#include "gui/prawrows.h"

#include "QDebug"

PRawRows::PRawRows(QObject *parent) :
    QObject(parent)
{

}

void PRawRows::observeWrite(const Token &token, size_t r, size_t c)
{
    emit written(createTokenVar(token), r, c);
}

void PRawRows::observeErase(const Region &r)
{
    emit erased(r.br, r.bc, r.er, r.ec);
}

void PRawRows::observeNewLine(size_t r, size_t c)
{
    emit newLine(r, c);
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

