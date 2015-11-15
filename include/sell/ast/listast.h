#ifndef LISTAST_H
#define LISTAST_H

#include "sell/ast/pigast.h"

class ListAst : public PigAst
{
public:
    ListAst(Type t);

    size_t size() const override;
    Ast &at(size_t pos) const override;

protected:
    void doInsert(size_t pos, Ast *child) override;
};

#endif // LISTAST_H
