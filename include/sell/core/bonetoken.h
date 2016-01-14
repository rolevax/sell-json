#ifndef BONETOKEN_H
#define BONETOKEN_H

#include "sell/core/token.h"

class BoneToken : public Token
{
public:
    BoneToken(const Ast *ast, const std::string &text, bool newline = false);

    const std::string &getText() const override;

private:
    std::string text;
};

#endif // BONETOKEN_H
