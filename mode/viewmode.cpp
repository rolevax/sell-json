#include "sell/mode/viewmode.h"
#include "sell/mode/menumode.h"
#include "sell/core/doc.h"

ViewMode::ViewMode(Doc &doc) :
    Mode(doc)
{

}

void ViewMode::keyboard(char key)
{
    switch (key) {
    case 'j':
        jackKick(+1);
        break;
    case 'k':
        jackKick(-1);
        break;
    case 'f':
        fuckIn();
        break;
    case 'd':
        damnOut();
        break;
    case 'a':
        if (outer->getType() == Ast::Type::OBJECT
                || outer->getType() == Ast::Type::ARRAY)
            ++inner;
        /* fall-through */
    case 'i':
        if (outer->getType() == Ast::Type::OBJECT
                || outer->getType() == Ast::Type::ARRAY)
            push(new MenuMode(doc));
        break;
    case 'x':
        remove();
        break;
    default:
        break;
    }

}

