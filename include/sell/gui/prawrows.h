#ifndef PRAWROWS_H
#define PRAWROWS_H

#include "sell/core/tokensobserver.h"
#include <QObject>
#include <QVariantList>
#include <QVariantMap>

class PRawRows : public QObject, public TokensObserver
{
    Q_OBJECT
public:
    explicit PRawRows(QObject *parent = 0);

    void observeUpdateLine(size_t r, const std::string &s) override;
    void observeNewLine(size_t r, size_t c) override;
    void observeJoinLine(size_t r) override;
    void observeRemoveLine(size_t r, size_t ct) override;
    void observeLight(size_t lbr, size_t lbc,
                      size_t ler, size_t lec,
                      size_t hbr, size_t hbc,
                      size_t her, size_t hec) override;
    void observeSetHotLight(bool b) override;

signals:
    void updateLine(int r, const QString str);
    void newLine(int r, int c);
    void joinLine(int r);
    void removeLine(int r, int ct);
    void hotLightSet(bool b);
    void lighted(int lbr, int lbc, int ler, int lec,
                 int hbr, int hbc, int her, int hec);

public slots:

private:
    static QVariant createTokenVar(const Token &token);
};

#endif // PRAWROWS_H
