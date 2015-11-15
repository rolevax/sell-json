#include "sell/ast/mapast.h"

#include <cassert>

MapAst::MapAst(Type t) :
    PigAst(t)
{
    // Supporting only JSON for now.
    assert(t == Type::PAIR);
    keys = { Type::KEY, Type::SCALAR };
}

size_t MapAst::size() const
{
    // TODO: might not strictly equal to container size
    return subtrees.size();
}

Ast &MapAst::at(size_t pos) const
{
    return *subtrees[pos];
}

void MapAst::doInsert(size_t pos, Ast *child)
{
    // TODO: subtype: assert(t is of type key[pos]);
    subtrees.emplace(subtrees.begin() + pos, std::move(child));
}

