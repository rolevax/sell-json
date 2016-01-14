#ifndef TOKENSOBSERVER_H
#define TOKENSOBSERVER_H

#include "token.h"

#include <qdebug.h>

class TokensObserver
{
public:
    TokensObserver() = default;

    virtual void observeNewLine(size_t /*r*/, size_t /*c*/) {}
    virtual void observeJoinLine(size_t /*r*/) {}
    virtual void observeWrite(const Token &/*token*/,
                               size_t /*r*/, size_t /*c*/) {}
    virtual void observeErase(const Region &/*r*/) {}
    virtual void observeUpdateFlesh(size_t /*r*/, size_t /*bc*/,
                                    size_t /*ec*/, const Token &/*t*/) {}
    virtual void observeSetHotLight(bool /*b*/) {}
    virtual void observeLight(size_t /*lbr*/, size_t /*lbc*/,
                              size_t /*ler*/, size_t /*lec*/,
                              size_t /*hbr*/, size_t /*hbc*/,
                              size_t /*her*/, size_t /*hec*/) {}
};

#endif // TOKENSOBSERVER_H
