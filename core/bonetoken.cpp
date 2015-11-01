#include "core/bonetoken.h"

#include <cassert>

BoneToken::BoneToken(const Ast *ast,
                     const std::string &text) :
    Token(ast, Role::BONE),
    text(text)
{
}

const std::string &BoneToken::getText() const
{
    return text;
}

