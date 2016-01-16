#include "sell/ast/listast.h"
#include "sell/ast/scalarast.h"

#include <algorithm>
#include <cassert>

ListAst::ListAst(Type t) :
    Ast(t)
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

std::unique_ptr<Ast> ListAst::remove(size_t pos)
{
    assert(pos < subtrees.size());
    std::unique_ptr<Ast> res = std::move(subtrees[pos]);
    subtrees.erase(subtrees.begin() + pos);
    return res;
}

size_t ListAst::indexOf(const Ast *child) const
{
    auto eq = [child](const std::unique_ptr<Ast> &a) {
        return a.get() == child;
    };
    auto it = std::find_if(subtrees.begin(), subtrees.end(), eq);

    if (it == subtrees.end())
        return -1;
    else
        return it - subtrees.begin();
}

void ListAst::doInsert(size_t pos, Ast *child)
{
    subtrees.emplace(subtrees.begin() + pos, child);
}

std::unique_ptr<Ast> ListAst::doChange(size_t pos, Ast *next)
{
    assert(pos < subtrees.size());
    std::unique_ptr<Ast> res = std::move(subtrees[pos]);
    subtrees[pos].reset(next);
    return res;
}

