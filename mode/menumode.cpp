#include "sell/mode/menumode.h"
#include "sell/mode/stringinputmode.h"
#include "sell/mode/numberinputmode.h"
#include "sell/mode/pairinputmode.h"
#include "sell/core/tokens.h"

#include <cassert>
#include <QDebug>

MenuMode::MenuMode(Doc &doc, Context context) :
    Mode(doc),
    context(context)
{

}

void MenuMode::keyboard(char key)
{
    switch (key) {
    case ' ':
        pop();
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
            && outer->getType() == Ast::Type::OBJECT;
    bool intoTyrant = context == Context::ASSART
            && outer->at(inner).getType() == Ast::Type::OBJECT;
    if (underTyrant || intoTyrant) {
        work(Ast::Type::PAIR);
        return;
    } else if (context == Context::NEST) {
        work(Ast::Type::ARRAY);
        return;
    }

    toggleTension(true);
}

void MenuMode::onPopped()
{
    toggleTension(false);
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
        change(type);
    } else if (context == Context::NEST) {
        nest(type);
    } else {
        // prepare cursor
        if (context == Context::APPEND) {
            ++inner;
        } else if (context == Context::ASSART) {
            outer = &outer->at(inner);
            inner = 0;
        }

        insert(type);
    }

    switch (type) {
    case Ast::Type::STRING:
        pop(new StringInputMode(doc, false));
        break;
    case Ast::Type::NUMBER:
        pop(new NumberInputMode(doc, false));
        break;
    case Ast::Type::PAIR:
        pop(new PairInputMode(doc));
        break;
    case Ast::Type::KEYTAL:
        assert(nullptr != keytal);
        while ('\0' != *keytal)
            static_cast<ScalarAst&>(outer->at(inner)).append(*keytal++);
        tokens.updateScalar(outer, inner);
        /* fall through */
    case Ast::Type::ARRAY:
    case Ast::Type::OBJECT:
        tokens.light(&outer->at(inner));
        pop();
        break;
    default:
        throw "MenuMode: work(): unhandled ast type";
        break;
    }
}


