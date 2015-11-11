#include "ast/pigast.h"
#include <algorithm>

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

size_t PigAst::indexOf(const Ast *child) const
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



