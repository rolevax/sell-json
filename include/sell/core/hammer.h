#ifndef HAMMER_H
#define HAMMER_H

#include "sell/ast/scalarast.h"
#include "sell/ast/listast.h"
#include "sell/ast/mapast.h"

class Token;
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

    void hitGeneral(const Ast &ast, size_t &r, size_t &c, bool tab, bool enter);
    void hitScalar(const ScalarAst &scalar, size_t &r, size_t &c, bool tab, bool enter);
    void hitList(const ListAst &list, size_t &r, size_t &c, bool tab, bool enter);
    void hitPair(const MapAst &pair, size_t &r, size_t &c);
    void indent(const Ast *master, size_t &r, size_t &c);

private:
    Tokens &tokens;
};

#endif // HAMMER_H
