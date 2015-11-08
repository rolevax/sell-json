#ifndef PRAWROWS_H
#define PRAWROWS_H

#include "core/tokensobserver.h"
#include <QObject>
#include <QVariantList>
#include <QVariantMap>

class PRawRows : public QObject, public TokensObserver
{
    Q_OBJECT
public:
    explicit PRawRows(QObject *parent = 0);

    void observeWrite(const Token &token, size_t r, size_t c) override;
    void observeErase(const Region &r) override;
    void observeNewLine(size_t r, size_t c) override;
    void observeLight(size_t lbr, size_t lbc,
                      size_t ler, size_t lec,
                      size_t hbr, size_t hbc,
                      size_t her, size_t hec) override;

signals:
    void written(const QVariant &token, int r, int c);
    void erased(int br, int bc, int er, int ec);
    void newLine(int r, int c);
    void lighted(int lbr, int lbc, int ler, int lec,
                 int hbr, int hbc, int her, int hec);

public slots:

private:
    static QVariant createTokenVar(const Token &token);
};

#endif // PRAWROWS_H
