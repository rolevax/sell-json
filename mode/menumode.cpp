#include "sell/mode/menumode.h"
#include "sell/mode/stringinputmode.h"
#include "sell/mode/numberinputmode.h"

MenuMode::MenuMode(Doc &doc, bool append, bool empty) :
    Mode(doc),
    append(append),
    empty(empty)
{

}

void MenuMode::keyboard(char key)
{
    switch (key) {
    case ' ':
        leave();
        break;
    case 's':
        prepareCursor();
        leave(new StringInputMode(doc));
        break;
    case 'n':
        prepareCursor();
        leave(new NumberInputMode(doc));
        break;
    case 'a':
        prepareCursor();
        insert(Ast::Type::ARRAY);
        leave();
        break;
    case 'o':
        prepareCursor();
        insert(Ast::Type::OBJECT);
        leave();
        break;
    default:
        break;
    }
}

void MenuMode::onPushed()
{
    // early leave if only one choice
    if (outer->getType() == Ast::Type::OBJECT
            || (empty && outer->at(inner).getType() == Ast::Type::OBJECT)) {
        prepareCursor();
        insert(Ast::Type::PAIR);
        leave();
        return;
    }

    const char *text = "s: string\n"
                       "n: number\n"
                       "a: array\n"
                       "o: object\n"
                       "space: back";
    showMenu(text);
}

void MenuMode::onPopped()
{
    showMenu("");
}

const char *MenuMode::name()
{
    return "Select Type";
}

void MenuMode::prepareCursor()
{
    if (append) {
        ++inner;
    } else if (empty) {
        outer = &outer->at(inner);
        inner = 0;
    }
}


