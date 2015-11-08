#include "ast/pigast.h"

#include <cassert>

PigAst::PigAst(Type t) :
    Ast(t)
{

}

std::unique_ptr<Ast> PigAst::remove(size_t pos)
{
    assert(pos < subtrees.size());
    std::unique_ptr<Ast> res = std::move(subtrees[pos]);
    subtrees.erase(subtrees.begin() + pos);
    return res;
}



