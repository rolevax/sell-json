#include "ast/listast.h"

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

void ListAst::doInsert(size_t pos, std::unique_ptr<Ast> &child)
{
    subtrees.insert(subtrees.begin() + pos, std::move(child));
}

