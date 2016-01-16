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
        if (Ast::isList(*outer)) {
            MenuMode::Context context;
            context = 'a' == key ? MenuMode::Context::APPEND
                                 : MenuMode::Context::INSERT;
            push(new MenuMode(doc, context));
        }
        break;
    case 'c':
        if (Ast::isChangeable(outer->at(inner))) {
            push(new MenuMode(doc, MenuMode::Context::CHANGE));
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


