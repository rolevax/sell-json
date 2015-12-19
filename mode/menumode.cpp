#include "sell/mode/menumode.h"
#include "sell/mode/stringinputmode.h"
#include "sell/mode/numberinputmode.h"
#include "sell/core/tokens.h"

#include <QDebug>

MenuMode::MenuMode(Doc &doc, Context context) :
    Mode(doc),
    context(context)
{

}

void MenuMode::keyboard(char key)
{
    /*
     * TODO: regard 'input mode' as 'modify mode',
     * insert inside this function only if mode is insert/append/assart
     */
    switch (key) {
    case ' ':
        leave();
        break;
    case 's':
        // TODO: make STRING type, do this with work()
        prepareCursor();
        insert(Ast::Type::SCALAR);
        leave(new StringInputMode(doc));
        break;
    case 'n':
        // TODO: make NUMBER type, do this with work()
        prepareCursor();
        insert(Ast::Type::SCALAR);
        leave(new NumberInputMode(doc));
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

/**
 * @brief MenuMode::prepareCursor
 * Call this only if next statement is "insert(type);"
 * TODO: consider move all these fucks into work()
 */
void MenuMode::prepareCursor()
{
    if (context == Context::APPEND) {
        ++inner;
    } else if (context == Context::ASSART) {
        outer = &outer->at(inner);
        inner = 0;
    }
}

/**
 * @brief MenuMode::work
 * @param type The type of the new node
 * Insert into or change inside 'outer', with
 * position specified by 'inner'.
 */
void MenuMode::work(Ast::Type type)
{
    // TODO: not just insert(), consider 'CHANGE' context
    switch (type) {
    case Ast::Type::SCALAR:
        // TODO: there should be not SCALAR (too super)
        break;
    case Ast::Type::ARRAY:
    case Ast::Type::OBJECT:
    case Ast::Type::PAIR:
        prepareCursor();
        insert(type);
        tokens.light(&outer->at(inner));
        leave();
        break;
    default:
        qDebug() << "Menumode: work(): unhandled type";
        break;
    }
}


