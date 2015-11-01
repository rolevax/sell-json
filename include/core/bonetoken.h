#ifndef BONETOKEN_H
#define BONETOKEN_H

#include "core/token.h"

class BoneToken : public Token
{
public:
    BoneToken(const Ast *ast, const std::string &text);

    const std::string &getText() const override;

private:
    std::string text;
};

#endif // BONETOKEN_H
