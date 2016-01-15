#ifndef MAPAST_H
#define MAPAST_H

#include "sell/ast/ast.h"
#include "sell/ast/scalarast.h"

class MapAst : public Ast
{
public:
    MapAst(Type t);

    size_t size() const override;
    Ast &at(size_t pos) const override;
    size_t indexOf(const Ast *child) const override;

protected:
    void doInsert(size_t pos, Ast *child) override;

private:
    std::unique_ptr<ScalarAst> key;
    std::unique_ptr<Ast> value;
};

#endif // MAPAST_H
