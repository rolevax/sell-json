#include "sell/mode/numberinputmode.h"
#include "sell/core/tokens.h"
#include "sell/ast/scalarast.h"

#include <cassert>

NumberInputMode::NumberInputMode(Doc &doc) :
    Mode(doc)
{

}

void NumberInputMode::keyboard(char key)
{
    assert(Ast::isScalar(outer->at(inner)));

    if (' ' == key) {
        leave();
        return;
    }

    char input;
    if (('0' <= key && key <= '9') || key == '\b')
        input = key;
    else if ('u' == key)
        input = '4';
    else if ('i' == key)
        input = '5';
    else if ('o' == key)
        input = '6';
    else if ('j' == key)
        input = '1';
    else if ('k' == key)
        input = '2';
    else if ('l' == key)
        input = '3';
    else if ('m' == key)
        input = '0';
    else
        return;

    ScalarAst &scalar = static_cast<ScalarAst&>(outer->at(inner));
    scalar.append(input);
    tokens.updateScalar(outer, inner);
    tokens.light(&outer->at(inner));

}

void NumberInputMode::onPushed()
{
    tokens.light(&outer->at(inner));
    tokens.setHotLight(true);
}

void NumberInputMode::onPopped()
{
    tokens.setHotLight(false);
}

const char *NumberInputMode::name()
{
    return "Number Input";
}

