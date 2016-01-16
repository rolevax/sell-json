#include "sell/core/soultoken.h"

#include <cassert>

const std::string SoulToken::empty;

SoulToken::SoulToken(const Ast *ast, Role role, bool newline) :
    Token(ast, role)
{
    assert(role == Role::BEGIN || role == Role::END || role == Role::META);
    newLine = newline;
}

const std::string &SoulToken::getText() const
{
    return empty;
}

