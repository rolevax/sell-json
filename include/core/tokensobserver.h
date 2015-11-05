#ifndef TOKENSOBSERVER_H
#define TOKENSOBSERVER_H

#include "token.h"

#include <qdebug.h>

class TokensObserver
{
public:
    TokensObserver() = default;

    virtual void observeCoord(size_t /*r*/, size_t /*c*/) {}
    virtual void observeNewRow() {}
    virtual void observeInsert(const Token &/*token*/) {}
    virtual void observeLight(size_t /*lbr*/, size_t /*lbc*/,
                              size_t /*ler*/, size_t /*lec*/,
                              size_t /*hbr*/, size_t /*hbc*/,
                              size_t /*her*/, size_t /*hec*/) {}
};

#endif // TOKENSOBSERVER_H