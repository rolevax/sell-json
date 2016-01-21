#include "sell/core/editabledoc.h"
#include "sell/mode/stringinputmode.h"
#include "sell/ast/scalarast.h"

#include <cassert>

StringInputMode::StringInputMode(EditableDoc &doc, bool clear) :
    Mode(doc),
    clear(clear)
{

}

void StringInputMode::keyboard(char key)
{
    switch (key) {
    case '\t':
    case '\r':
        doc.pop();
        break;
    default:
        assert(Ast::isScalar(doc.innerType()));
        doc.scalarAppend(key);
        doc.light();
        break;
    }
}

void StringInputMode::onPushed()
{
    if (clear) {
        assert(Ast::isScalar(doc.innerType()));
        doc.scalarClear();
    }

    doc.light();
    doc.setHotLight(true);
}

void StringInputMode::onPopped()
{
    doc.setHotLight(false);
}

const char *StringInputMode::name()
{
    return "String Input";
}

