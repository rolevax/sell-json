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

    void write(const Ast &ast, size_t r, size_t c);

private:
    void write(Token *token, size_t &r, size_t &c);
    void newLine(size_t &r, size_t &c);

    void writeGeneral(const Ast &ast, size_t &r, size_t &c);
    void writeScalar(const ScalarAst &scalar, size_t &r, size_t &c);
    void writeObject(const ListAst &object, size_t &r, size_t &c);
    void writeArray(const ListAst &array, size_t &r, size_t &c);
    void writePair(const MapAst &pair, size_t &r, size_t &c);
    void indent(const Ast *master, size_t &r, size_t &c);

private:
    Tokens &tokens;
};

#endif // HAMMER_H
