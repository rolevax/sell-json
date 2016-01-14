#ifndef SOULTOKEN_H
#define SOULTOKEN_H

#include "sell/core/token.h"

class SoulToken : public Token
{
public:
    SoulToken(const Ast *ast, Role role, bool newLine = false);

    const std::string &getText() const;

private:
    static const std::string empty;
};

#endif // SOULTOKEN_H
