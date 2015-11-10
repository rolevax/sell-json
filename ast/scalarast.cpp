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
    return 0;
}

Ast &ScalarAst::at(size_t pos) const
{
    (void) pos;
    throw -1;
}

std::unique_ptr<Ast> ScalarAst::remove(size_t pos)
{
    (void) pos;
    throw 445;
}

void ScalarAst::doInsert(size_t pos, std::unique_ptr<Ast> &child)
{
    (void) pos;
    (void) child;
    throw -1;
}

