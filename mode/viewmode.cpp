#include "sell/mode/viewmode.h"
#include "sell/mode/menumode.h"
#include "sell/mode/stringinputmode.h"
#include "sell/mode/numberinputmode.h"
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
        } else if (outer->getType() == Ast::Type::ROOT) {
            push(new MenuMode(doc, MenuMode::Context::INSERT));
        }
        break;
    case 'c':
        if (Ast::isChangeable(outer->at(inner))) {
            push(new MenuMode(doc, MenuMode::Context::CHANGE));
        }
        break;
    case 'n':
        if (Ast::isChangeable(outer->at(inner))) {
            push(new MenuMode(doc, MenuMode::Context::NEST));
        }
        break;
    case 'm':
        switch (outer->at(inner).getType()) {
        case Ast::Type::STRING:
        case Ast::Type::KEY:
            push(new StringInputMode(doc));
            break;
        case Ast::Type::NUMBER:
            push(new NumberInputMode(doc));
            break;
        default:
            break;
        }
        break;
    case 'x':
        if (Ast::isList(*outer) || outer->getType() == Ast::Type::ROOT)
            remove();
        break;
    default:
        break;
    }
}

void ViewMode::emptyKeyboard(char key)
{
    if (key == 'i')
        keyboard(key);
}

const char *ViewMode::name()
{
    return "View";
}


