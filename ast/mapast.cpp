#include "ast/mapast.h"

#include <cassert>

MapAst::MapAst(Type t) :
    PigAst(t)
{
    // Supporting only JSON for now.
    assert(t == Type::PAIR);
    keys = { Type::KEY, Type::SCALAR };
    subtrees.resize(2);
}

size_t MapAst::size() const
{
    // TODO: dummy, consider carefully (last empty child?)
    return subtrees.size();
}

Ast &MapAst::at(size_t pos) const
{
    return *subtrees[pos];
}

void MapAst::doInsert(size_t pos, std::unique_ptr<Ast> &child)
{
    // TODO: impl subtype func?
    // Type t(child->getType());
    // assert(t == Type::VALUE || t == keys[pos]);
    // TODO: leave blank if jump insert (need?)
    subtrees.insert(subtrees.begin() + pos, std::move(child));
}

