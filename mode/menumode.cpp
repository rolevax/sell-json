#include "sell/core/editabledoc.h"
#include "sell/mode/menumode.h"
#include "sell/mode/stringinputmode.h"
#include "sell/mode/numberinputmode.h"
#include "sell/mode/pairinputmode.h"

#include <cassert>
#include <QDebug>

MenuMode::MenuMode(EditableDoc &doc, Context context) :
    Mode(doc),
    context(context)
{

}

void MenuMode::keyboard(char key)
{
    switch (key) {
    case ' ':
        doc.pop();
        break;
    case 's':
        work(Ast::Type::STRING);
        break;
    case 'n':
        work(Ast::Type::NUMBER);
        break;
    case 'x':
        work(Ast::Type::KEYTAL, "null");
        break;
    case 't':
        work(Ast::Type::KEYTAL, "true");
        break;
    case 'f':
        work(Ast::Type::KEYTAL, "false");
        break;
    case 'a':
        work(Ast::Type::ARRAY);
        break;
    case 'o':
        work(Ast::Type::OBJECT);
        break;
    default:
        break;
    }
}

void MenuMode::emptyKeyboard(char key)
{
    keyboard(key);
}

void MenuMode::onPushed()
{
    // early leave if only one choice
    bool underTyrant = context != Context::ASSART
            && doc.outerType() == Ast::Type::OBJECT;
    bool intoTyrant = context == Context::ASSART
            && doc.innerType() == Ast::Type::OBJECT;
    if (underTyrant || intoTyrant) {
        work(Ast::Type::PAIR);
        return;
    } else if (context == Context::NEST) {
        work(Ast::Type::ARRAY);
        return;
    }

    doc.toggleTension(true);
}

void MenuMode::onPopped()
{
    doc.toggleTension(false);
}

const char *MenuMode::name()
{
    return "Select Type";
}

/**
 * @brief MenuMode::work
 * @param type The type of the new node
 * Insert into or change inside 'outer', with
 * position specified by 'inner'.
 */
void MenuMode::work(Ast::Type type, const char *keytal)
{
    if (context == Context::CHANGE) {
        doc.change(type);
    } else if (context == Context::NEST) {
        doc.nest(type);
    } else {
        // prepare cursor
        if (context == Context::APPEND) {
            doc.cursorForward();
        } else if (context == Context::ASSART) {
            doc.cursorIn();
        }

        doc.insert(type);
    }

    switch (type) {
    case Ast::Type::STRING:
        doc.pop(new StringInputMode(doc, false));
        break;
    case Ast::Type::NUMBER:
        doc.pop(new NumberInputMode(doc, false));
        break;
    case Ast::Type::PAIR:
        doc.pop(new PairInputMode(doc));
        break;
    case Ast::Type::KEYTAL:
        assert(nullptr != keytal);
        doc.scalarAppend(keytal);
        /* fall through */
    case Ast::Type::ARRAY:
    case Ast::Type::OBJECT:
        doc.pop();
        break;
    default:
        throw "MenuMode: work(): unhandled ast type";
        break;
    }
}


