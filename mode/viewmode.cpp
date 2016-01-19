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
    // abstract cursor moving
    case 'g': // get next node
        sibling(+1);
        break;
    case 's': // senior previous node
        sibling(-1);
        break;
    case 'f': // fall in
        fuckIn();
        break;
    case 'd': // dig out
        damnOut();
        break;

    // concrete cursor moving
    case 'h': // hack left
        if (outer->getType() == Ast::Type::PAIR && inner == 1)
            sibling(-1);
        break;
    case 'l': // lead right
        if (outer->getType() == Ast::Type::PAIR && inner == 0)
            sibling(+1);
        break;
    case 'j': // jack down
        jackKick(true);
        break;
    case 'k': // kick up
        jackKick(false);
        break;

    // outer modification
    case 'o': // oh, append
    case 'i': // insert
        if (Ast::isList(*outer)) {
            MenuMode::Context context;
            context = 'o' == key ? MenuMode::Context::APPEND
                                 : MenuMode::Context::INSERT;
            push(new MenuMode(doc, context));
        } else if (outer->getType() == Ast::Type::ROOT) {
            push(new MenuMode(doc, MenuMode::Context::INSERT));
        }
        break;
    case 'r': // remove
        if (Ast::isList(*outer) || outer->getType() == Ast::Type::ROOT)
            remove();
        // TODO get to clipboard
        break;
    case 'y': // yank
        // TODO
        break;
    case 'p': // paste
        break;

    // inner modification
    case 'c': // change
        if (Ast::isChangeable(outer->at(inner))) {
            push(new MenuMode(doc, MenuMode::Context::CHANGE));
        }
        break;
    case 'n': // nest
        if (Ast::isChangeable(outer->at(inner))) {
            push(new MenuMode(doc, MenuMode::Context::NEST));
        }
        break;
    case 'm': // modify
    case 'M':
        switch (outer->at(inner).getType()) {
        case Ast::Type::STRING:
        case Ast::Type::KEY:
            push(new StringInputMode(doc, key == 'M'));
            break;
        case Ast::Type::NUMBER:
            push(new NumberInputMode(doc, key == 'M'));
            break;
        default:
            break;
        }
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


