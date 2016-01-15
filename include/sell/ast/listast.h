#ifndef LISTAST_H
#define LISTAST_H

#include "sell/ast/ast.h"

#include <vector>

class ListAst : public Ast
{
public:
    ListAst(Type t);

    size_t size() const override;
    Ast &at(size_t pos) const override;
    std::unique_ptr<Ast> remove(size_t pos) override;
    size_t indexOf(const Ast *child) const override;

protected:
    void doInsert(size_t pos, Ast *child) override;

private:
    std::vector<std::unique_ptr<Ast>> subtrees;
};

#endif // LISTAST_H
