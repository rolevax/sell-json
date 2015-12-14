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
    case 'i':
        if (outer->getType() == Ast::Type::OBJECT
                || outer->getType() == Ast::Type::ARRAY) {
            MenuMode::Context context;
            context = 'a' == key ? MenuMode::Context::APPEND
                                 : MenuMode::Context::INSERT;
            push(new MenuMode(doc, context));
        }
        break;
    case 'x':
        remove();
        break;
    default:
        break;
    }

}

const char *ViewMode::name()
{
    return "View";
}


