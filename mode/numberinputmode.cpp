#include "sell/core/editabledoc.h"
#include "sell/mode/numberinputmode.h"
#include "sell/ast/scalarast.h"

#include <cassert>

NumberInputMode::NumberInputMode(EditableDoc &doc, bool clear) :
    Mode(doc),
    clear(clear)
{

}

void NumberInputMode::keyboard(char key)
{
    assert(doc.innerType() == Ast::Type::NUMBER);

    if (' ' == key) {
        doc.pop();
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

    doc.scalarAppend(input);
    doc.light();
}

void NumberInputMode::onPushed()
{
    if (clear) {
        assert(doc.innerType() == Ast::Type::NUMBER);
        doc.scalarClear();
    }

    doc.light();
    doc.setHotLight(true);
}

void NumberInputMode::onPopped()
{
    doc.setHotLight(false);
}

const char *NumberInputMode::name()
{
    return "Number Input";
}

