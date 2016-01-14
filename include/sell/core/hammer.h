#ifndef HAMMER_H
#define HAMMER_H

#include "sell/ast/scalarast.h"
#include "sell/ast/listast.h"
#include "sell/ast/mapast.h"

class Tokens;

class Hammer
{
public:
    Hammer(Tokens &tokens);

    Hammer(const Hammer&) = delete;
    Hammer &operator=(const Hammer&) = delete;

    void hit(const Ast &ast, size_t r, size_t c);

private:
    void write(Token *token, size_t &r, size_t &c);

    void hitGeneral(const Ast &ast, size_t &r, size_t &c);
    void hitScalar(const ScalarAst &scalar, size_t &r, size_t &c);
    void hitObject(const ListAst &object, size_t &r, size_t &c);
    void hitArray(const ListAst &array, size_t &r, size_t &c);
    void hitPair(const MapAst &pair, size_t &r, size_t &c);
    void indent(const Ast *master, size_t &r, size_t &c);

private:
    Tokens &tokens;
};

#endif // HAMMER_H
