#include "sell/ast/listast.h"

ListAst::ListAst(Type t) :
    PigAst(t)
{

}

size_t ListAst::size() const
{
    return subtrees.size();
}

Ast &ListAst::at(size_t pos) const
{
    return *subtrees[pos];
}

void ListAst::doInsert(size_t pos, Ast *child)
{
    subtrees.emplace(subtrees.begin() + pos, child);
}

