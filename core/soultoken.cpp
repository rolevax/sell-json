#include "sell/core/soultoken.h"

#include <cassert>

const std::string SoulToken::empty;

SoulToken::SoulToken(const Ast *ast, Role role) :
    Token(ast, role)
{
    assert(role == Role::BEGIN || role == Role::END);
}

const std::string &SoulToken::getText() const
{
    return empty;
}

