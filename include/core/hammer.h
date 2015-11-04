#ifndef HAMMER_H
#define HAMMER_H

#include "core/tokens.h"
#include "ast/scalarast.h"
#include "ast/listast.h"
#include "ast/mapast.h"

class Hammer
{
public:
    Hammer(Tokens &rawRows);

    Hammer(const Hammer&) = delete;
    Hammer &operator=(const Hammer&) = delete;

    void write(const Ast &value);
    void writeScalar(const ScalarAst &scalar);
    void writeObject(const ListAst &object);
    void writeArray(const ListAst &array);

private:
    void indent(const Ast *master);

private:
    Tokens &rawRows;
    int indentLevel = 0;
};

#endif // HAMMER_H
