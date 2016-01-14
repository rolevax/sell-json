#include "sell/core/bonetoken.h"

#include <cassert>

BoneToken::BoneToken(const Ast *ast,
                     const std::string &text, bool newline) :
    Token(ast, Role::BONE),
    text(text)
{
    newLine = newline;
}

const std::string &BoneToken::getText() const
{
    return text;
}

