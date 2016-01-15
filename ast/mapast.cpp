#include "sell/ast/mapast.h"

#include <cassert>

MapAst::MapAst(Type t) :
    Ast(t)
{
    // Supporting only JSON for now.
    assert(t == Type::PAIR);
}

size_t MapAst::size() const
{
    // TODO: might not strictly equal to container size
    return 2;
}

Ast &MapAst::at(size_t pos) const
{
    if (pos > 1)
        throw 880;
    return pos == 0 ? *key : *value;
}

size_t MapAst::indexOf(const Ast *child) const
{
    return key.get() == child ? 0
                              : value.get() == child ? 1 : -1;
}

void MapAst::doInsert(size_t pos, Ast *child)
{
    if (pos == 0) {
        assert(child->getType() == Ast::Type::KEY);
        key.reset(static_cast<ScalarAst*>(child));
    } else if (pos == 1) {
        value.reset(child);
    } else {
        throw 789;
    }
}

