#include "sell/mode/menumode.h"
#include "sell/mode/stringinputmode.h"
#include "sell/mode/numberinputmode.h"

MenuMode::MenuMode(Doc &doc, bool append) :
    Mode(doc),
    append(append)
{

}

void MenuMode::keyboard(char key)
{
    switch (key) {
    case ' ':
        leave();
        break;
    case 's':
        if (append)
            ++inner;
        leave(new StringInputMode(doc));
        break;
    case 'n':
        if (append)
            ++inner;
        leave(new NumberInputMode(doc));
        break;
    case 'a':
        if (append)
            ++inner;
        insert(Ast::Type::ARRAY);
        leave();
        break;
    case 'o':
        if (append)
            ++inner;
        insert(Ast::Type::OBJECT);
        leave();
        break;
    default:
        break;
    }
}

void MenuMode::onPushed()
{
    if (outer->getType() == Ast::Type::OBJECT) {
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


