#include "ast/scalarast.h"

ScalarAst::ScalarAst(Type t, const std::string &text) :
    Ast(t),
    text(text)
{

}

const std::string &ScalarAst::getText() const
{
    return text;
}

size_t ScalarAst::size() const
{
    return 0; // TODO: raise?
}

Ast &ScalarAst::at(size_t pos) const
{
    (void) pos;
    throw -1; // TODO
}

void ScalarAst::doInsert(size_t pos, std::unique_ptr<Ast> &child)
{
    (void) pos;
    (void) child;
    throw -1; // TODO
}

