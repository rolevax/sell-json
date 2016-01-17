#ifndef MAPAST_H
#define MAPAST_H

#include "sell/ast/ast.h"

class MapAst : public Ast
{
public:
    MapAst(Type t);

    size_t size() const override;
    Ast &at(size_t pos) const override;
    std::unique_ptr<Ast> remove(size_t pos) override;
    size_t indexOf(const Ast *child) const override;

protected:
    void doInsert(size_t pos, Ast *child) override;
    std::unique_ptr<Ast> doChange(size_t pos, Ast *next) override;

private:
    std::unique_ptr<Ast> key;
    std::unique_ptr<Ast> value;
};

#endif // MAPAST_H
