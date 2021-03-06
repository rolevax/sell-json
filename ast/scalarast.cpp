#include "sell/ast/scalarast.h"

ScalarAst::ScalarAst(Type t, const std::string &text) :
    Ast(t),
    text(text)
{

}

ScalarAst *ScalarAst::clone() const
{
    ScalarAst *ret = new ScalarAst(type, text);
    return ret;
}

const std::string &ScalarAst::getText() const
{
    return text;
}

void ScalarAst::append(char c)
{
    if (c == '\b') {
        text.pop_back();
    } else {
        text += c;
    }
}

void ScalarAst::clear()
{
    text.clear();
}


