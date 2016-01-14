#include "sell/core/token.h"

Token::Token(const Ast *ast, Role role) :
    ast(ast),
    role(role)
{

}

const Ast *Token::getAst() const
{
    return ast;
}

Token::Role Token::getRole() const
{
    return role;
}

bool Token::needNewLine() const
{
    return newLine;
}


