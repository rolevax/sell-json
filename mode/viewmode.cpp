#include "sell/core/editabledoc.h"
#include "sell/mode/viewmode.h"
#include "sell/mode/menumode.h"
#include "sell/mode/stringinputmode.h"
#include "sell/mode/numberinputmode.h"

ViewMode::ViewMode(EditableDoc &doc) :
    Mode(doc)
{

}

void ViewMode::keyboard(char key)
{
    switch (key) {
    // abstract cursor moving
    case 'g': // get next node
        doc.sibling(+1);
        break;
    case 's': // senior previous node
        doc.sibling(-1);
        break;
    case 'f': // fall in
        doc.fallIn();
        break;
    case 'd': // dig out
        doc.digOut();
        break;

    // concrete cursor moving
    case 'h': // hack left
        doc.hackLead(false);
        break;
    case 'l': // lead right
        doc.hackLead(true);
        break;
    case 'k': // kick up
        doc.jackKick(false);
        break;
    case 'j': // jack down
        doc.jackKick(true);
        break;

    // outer modification
    case 'o': // oh, append
    case 'i': // insert
        if (Ast::isList(doc.outerType())) {
            MenuMode::Context context;
            context = 'o' == key ? MenuMode::Context::APPEND
                                 : MenuMode::Context::INSERT;
            doc.push(new MenuMode(doc, context));
        }
        break;
    case 'r': // remove
        if (Ast::isList(doc.outerType()) || doc.outerType() == Ast::Type::ROOT)
            doc.remove();
        // TODO get to clipboard
        break;
    case 'y': // yank
        // TODO
        break;
    case 'p': // paste
        break;

    // inner modification
    case 'c': // change
        if (Ast::isChangeable(doc.innerType())) {
            doc.push(new MenuMode(doc, MenuMode::Context::CHANGE));
        }
        break;
    case 'n': // nest
        if (Ast::isChangeable(doc.innerType())) {
            doc.push(new MenuMode(doc, MenuMode::Context::NEST));
        }
        break;
    case 'm': // modify
    case 'M':
        switch (doc.innerType()) {
        case Ast::Type::STRING:
        case Ast::Type::KEY:
            doc.push(new StringInputMode(doc, key == 'M'));
            break;
        case Ast::Type::NUMBER:
            doc.push(new NumberInputMode(doc, key == 'M'));
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
    // insert to empty document
    if (key == 'i')
        doc.push(new MenuMode(doc, MenuMode::Context::INSERT));
}

const char *ViewMode::name()
{
    return "View";
}


