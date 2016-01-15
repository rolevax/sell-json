// SELL headers
#include "sell/core/doc.h"
#include "sell/mode/viewmode.h"
#include "sell/mode/menumode.h"
#include "sell/ast/astconverter.h"
#include "sell/gui/pdoc.h"

// RapidJSON headers
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/error/en.h"

// headers for file I/O
#include <fstream>
#include <sstream>

// headers for debug uses
#include <QDebug>
#include <iostream>
#include <cstdlib>

Doc::Doc()
{
    modes.emplace(new ViewMode(*this));
}

void Doc::load()
{
    assert(!root.present());

    std::ifstream ifs("sample.json");
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string str(ss.str());
    const char *json = str.c_str();

    rapidjson::Document d;
    if (d.Parse(json).HasParseError()) {
        std::cerr << "Parse error: "
                  << GetParseError_En(d.GetParseError())
                  << std::endl;
        exit(1);
    }

    AstConverter ac(root);
    ac.convert(d);

    tokens.insert(outer, inner);
    tokens.light(&outer->at(inner));

//    tokens.print();
}

/**
 * @brief Doc::keyboard
 * @param key
 * Accept a key stroke and handle it with the mode on
 * the top of the mode stack.
 */
void Doc::keyboard(char key)
{
    assert(modes.size() > 0);
    modes.top()->keyboard(key);
}

void Doc::registerTokensObserver(TokensObserver *ob)
{
    tokens.registerObserver(ob);
}

void Doc::registerObserver(PDoc *ob)
{
    this->ob = ob;
}

/**
 * @brief Doc::push
 * @param mode
 * Push 'mode' onto the mode stack,
 * and trigger the 'onPushed' callback of 'mode'
 * This will take away the ownership of 'mode'.
 */
void Doc::push(Mode *mode)
{
    modes.emplace(mode);
    if (ob != nullptr)
        ob->observePush(modes.top()->name());
    modes.top()->onPushed();
}

/**
 * @brief Doc::pop
 * Pop the top of the mode stack,
 * and call the 'onPopped' callback of that mode.
 * Afterwards that mode object will be destructed.
 */
void Doc::pop()
{
    if (ob != nullptr)
        ob->observePop();

    std::unique_ptr<Mode> popped = std::move(modes.top());
    modes.pop();
    popped->onPopped();
}

void Doc::fuckIn()
{
    assert(inner < outer->size());

    Ast &focus = outer->at(inner);
    Ast::Type type = focus.getType();

    if (focus.size() == 0) {
        if (Ast::isList(type)) {
            push(new MenuMode(*this, MenuMode::Context::ASSART));
        }
        return;
    }

    switch (type) {
    case Ast::Type::ARRAY:
    case Ast::Type::OBJECT:
    case Ast::Type::PAIR:
    case Ast::Type::ROOT:
        outer = &focus;
        inner = 0;
        tokens.light(&outer->at(inner));
        break;
    case Ast::Type::KEY:
    case Ast::Type::STRING:
    case Ast::Type::NUMBER:
    case Ast::Type::KEYTAL:
        assert("size of key or scalar should be 0" && false);
        break;
    }
}

void Doc::damnOut()
{
    if (outer == &root) {
        qDebug() << "cannot damnout root";
        return;
    }

    Ast *nextOuter = &outer->getParent();
    inner = nextOuter->indexOf(outer);
    tokens.light(outer);
    outer = nextOuter;
}

void Doc::jackKick(int step)
{
    size_t size = outer->size();
    inner = (ssize_t(inner + size) + step) % size;
    tokens.light(&outer->at(inner));
}

/**
 * @brief Doc::insert Create a new subnode
 * @param type The type of the new node
 * Create a new subnode inside 'outer' at 'inner',
 * with specified type 'type'.
 * The value of the new node is "as empty as possible".
 * ... number and keytal, is "", makes it inconsistent, might be a bad design
 */
void Doc::insert(Ast::Type type)
{
    assert(inner <= outer->size());

    Ast *a;

    switch (type) {
    case Ast::Type::PAIR:
        a = new MapAst(Ast::Type::PAIR);
        a->insert(0, new ScalarAst(Ast::Type::KEY, "__key"));
        a->insert(1, new ScalarAst(Ast::Type::KEYTAL, "null"));
        break;
    case Ast::Type::STRING:
        a = new ScalarAst(Ast::Type::STRING, "");
        break;
    case Ast::Type::NUMBER:
        a = new ScalarAst(Ast::Type::NUMBER, "");
        break;
    case Ast::Type::KEYTAL:
        a = new ScalarAst(Ast::Type::KEYTAL, "");
        break;
    case Ast::Type::ARRAY:
        a = new ListAst(Ast::Type::ARRAY);
        break;
    case Ast::Type::OBJECT:
        a = new ListAst(Ast::Type::OBJECT);
        break;
    case Ast::Type::ROOT:
    case Ast::Type::KEY:
        qDebug() << "insert type: untreated outer type";
        break;
    }

    assert(a != nullptr);

    outer->insert(inner, a);
    tokens.insert(outer, inner);
}

void Doc::remove()
{
    assert(inner < outer->size());

    if (!Ast::isList(*outer)) {
        qDebug() << "unremovable outer";
        return; // TODO: allow removing child of root
    }
    tokens.remove(outer, inner);
    outer->remove(inner);
    if (inner == outer->size()) {
        if (inner > 0) {
            --inner;
        } else { // outer became empty
            damnOut(); // TODO: remove child of root case
            // re-insert to generate special empty look
            tokens.remove(outer, inner);
            tokens.insert(outer, inner);
        }
    }
    tokens.light(&outer->at(inner));
}

void Doc::showMenu(const char *text)
{
    if (ob != nullptr)
        ob->observeMenu(text);
}

