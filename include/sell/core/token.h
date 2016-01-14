#ifndef TOKEN_H
#define TOKEN_H

#include "sell/ast/ast.h"
#include <string>

struct Region
{
    size_t br;
    size_t bc;
    size_t er;
    size_t ec;
};

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
    bool needNewLine() const;

protected:
    const Ast *ast;
    Role role;
    bool newLine = false;
};

#endif // TOKEN_H
