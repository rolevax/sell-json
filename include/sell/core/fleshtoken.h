#ifndef FLESHTOKEN_H
#define FLESHTOKEN_H

#include "sell/core/token.h"
#include "sell/ast/scalarast.h"

class FleshToken : public Token
{
public:
    FleshToken(const ScalarAst *ast);

    const std::string &getText() const override;

private:
    const ScalarAst *scalar;
};

#endif // FLESHTOKEN_H
