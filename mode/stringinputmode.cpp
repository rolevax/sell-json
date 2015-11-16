#include "sell/mode/stringinputmode.h"
#include "sell/core/tokens.h"
#include "sell/ast/scalarast.h"

#include <cassert>

StringInputMode::StringInputMode(Doc &doc) :
    Mode(doc)
{

}

void StringInputMode::keyboard(char key)
{
    switch (key) {
    case ' ':
        leave();
        break;
    default:
        assert(outer->at(inner).getType() == Ast::Type::SCALAR);
        ScalarAst &scalar = static_cast<ScalarAst&>(outer->at(inner));
        scalar.append(key);
        tokens.updateScalar(outer, inner);
        tokens.light(&outer->at(inner));
        break;
    }
}

void StringInputMode::onPushed()
{
    insert(Ast::Type::SCALAR);
    tokens.light(&outer->at(inner));
    tokens.setHotLight(true);
}

void StringInputMode::onPopped()
{
    tokens.setHotLight(false);
}

