#include "sell/ast/rootast.h"

#include <cassert>

RootAst::RootAst() :
    Ast(Type::ROOT)
{
    parent = this;
    root = this;
}

bool RootAst::present() const
{
    return subtree != nullptr;
}

size_t RootAst::size() const
{
    return present() ? 1 : 0;
}

Ast &RootAst::at(size_t pos) const
{
    assert(pos == 0 && present());
    return *subtree;
}

std::unique_ptr<Ast> RootAst::remove(size_t pos)
{
    (void) pos;
    throw 444;
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
    assert(!present() && pos == 0);
    subtree.reset(child);
}

