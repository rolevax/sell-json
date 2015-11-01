#ifndef TOKEN_H
#define TOKEN_H

#include "ast/ast.h"
#include <string>

class Token
{
public:
    enum class Role {
        BEGIN, END, BONE, FLESH
    };

    Token(const Ast *ast, Role role);

    const Ast *getAst() const;
    Role getRole() const;
    virtual const std::string &getText() const = 0;

protected:
    const Ast *ast;
    Role role;
};

#endif // TOKEN_H
