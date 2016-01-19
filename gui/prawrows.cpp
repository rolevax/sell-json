#include "sell/core/tokens.h"
#include "sell/gui/prawrows.h"

#include "QDebug"

PRawRows::PRawRows(QObject *parent) :
    QObject(parent)
{

}

void PRawRows::observeUpdateLine(size_t r, const std::string &s)
{
    emit updateLine(r, QString::fromStdString(s));
}

void PRawRows::observeNewLine(size_t r, size_t c)
{
    emit newLine(r, c);
}

void PRawRows::observeJoinLine(size_t r)
{
    emit joinLine(r);
}

void PRawRows::observeRemoveLine(size_t r, size_t ct)
{
    emit removeLine(r, ct);
}

void PRawRows::observeLight(size_t lbr, size_t lbc,
                            size_t ler, size_t lec,
                            size_t hbr, size_t hbc,
                            size_t her, size_t hec)
{
    emit lighted(lbr, lbc, ler, lec, hbr, hbc, her, hec);
}

void PRawRows::observeSetHotLight(bool b)
{
    emit hotLightSet(b);
}

QVariant PRawRows::createTokenVar(const Token &token)
{
    QVariantMap map;
    map.insert("modelText", QString::fromStdString(token.getText()));
    return QVariant::fromValue(map);
}

