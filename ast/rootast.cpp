#include "include/ast/rootast.h"

#include <cassert>

RootAst::RootAst() :
    Ast(Type::ROOT)
{
    parent = this;
    root = this;
}

bool RootAst::isPresent() const
{
    return present;
}

size_t RootAst::size() const
{
    return present ? 1 : 0;
}

Ast &RootAst::at(size_t pos) const
{
    assert(pos == 0 && present);
    return *subtree;
}

void RootAst::doInsert(size_t pos, std::unique_ptr<Ast> &child)
{
    assert(pos == 0 && !present);
    subtree = std::move(child);
    present = true;
}

