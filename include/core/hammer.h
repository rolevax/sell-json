#ifndef HAMMER_H
#define HAMMER_H

#include "ast/scalarast.h"
#include "ast/listast.h"
#include "ast/mapast.h"

class Tokens;

class Hammer
{
public:
    Hammer(Tokens &tokens);

    Hammer(const Hammer&) = delete;
    Hammer &operator=(const Hammer&) = delete;

    void write(const Ast &ast);

private:
    void writeScalar(const ScalarAst &scalar);
    void writeObject(const ListAst &object);
    void writeArray(const ListAst &array);
    void indent(const Ast *master);

private:
    Tokens &tokens;
};

#endif // HAMMER_H
