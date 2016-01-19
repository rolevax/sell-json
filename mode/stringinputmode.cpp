#include "sell/mode/stringinputmode.h"
#include "sell/core/tokens.h"
#include "sell/ast/scalarast.h"

#include <cassert>

StringInputMode::StringInputMode(Doc &doc, bool clear) :
    Mode(doc),
    clear(clear)
{

}

void StringInputMode::keyboard(char key)
{
    switch (key) {
    case '\t':
    case '\r':
        pop();
        break;
    default:
        assert(Ast::isScalar(outer->at(inner)));
        ScalarAst &scalar = static_cast<ScalarAst&>(outer->at(inner));
        scalar.append(key);
        tokens.updateScalar(outer, inner);
        tokens.light(&outer->at(inner));
        break;
    }
}

void StringInputMode::onPushed()
{
    if (clear) {
        assert(Ast::isScalar(outer->at(inner)));
        ScalarAst &scalar = static_cast<ScalarAst&>(outer->at(inner));
        scalar.clear();
        tokens.updateScalar(outer, inner);
    }

    tokens.light(&outer->at(inner));
    tokens.setHotLight(1); // 1-back to skip quote
}

void StringInputMode::onPopped()
{
    tokens.setHotLight(-1); // turn off hot light
}

const char *StringInputMode::name()
{
    return "String Input";
}

