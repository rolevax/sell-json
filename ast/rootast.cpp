#include "sell/ast/rootast.h"

#include <cassert>

RootAst::RootAst() :
    Ast(Type::ROOT)
{
    parent = this;
    root = this;
}

size_t RootAst::size() const
{
    return subtree == nullptr ? 0 : 1;
}

Ast &RootAst::at(size_t pos) const
{
    assert(pos < size());
    return *subtree;
}

std::unique_ptr<Ast> RootAst::remove(size_t pos)
{
    assert(pos < size());
    return std::move(subtree);
}

size_t RootAst::indexOf(const Ast *child) const
{
    if (subtree && subtree.get() == child)
        return 0;
    else
        return -1;
}

void RootAst::doInsert(size_t pos, Ast *child)
{
    assert(pos == 0 && size() == 0);
    subtree.reset(child);
}

std::unique_ptr<Ast> RootAst::doChange(size_t pos, Ast *next)
{
    assert(pos == 0);
    std::unique_ptr<Ast> ret = std::move(subtree);
    subtree.reset(next);
    return ret;
}

