#include "sell/core/fleshtoken.h"

FleshToken::FleshToken(const ScalarAst *ast) :
    Token(ast, Role::FLESH),
    scalar(ast)
{

}

const std::string &FleshToken::getText() const
{
    return scalar->getText();
}

