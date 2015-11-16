#include "sell/core/doc.h"
#include "sell/mode/viewmode.h"
#include "sell/mode/menumode.h"
#include "sell/ast/astconverter.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

#include <chrono>
#include <QDebug>
#include <iostream>

Doc::Doc()
{
    modes.emplace(new ViewMode(*this));
}

void Doc::load()
{
    assert(!root.present());
    const char *json = "[{"
                       "\"name\": \"you\","
                       "\"age\": 18,"
                       "\"fucked\": true"
                       "},"
                       "123,"
                       "[34, 79, [98, 23], 90],"
                       "[],{},9.88"
                       "]";
    rapidjson::Document d;
    d.Parse(json);
    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> pw(sb);
    d.Accept(pw);
    qDebug() << sb.GetString();

    AstConverter ac(root);
    ac.convert(d);

    tokens.insert(outer, inner);
//    tokens.light(&outer->at(inner));

    tokens.print();
}

/*
 * TODO XXX
 * MenuMode has append:bool
 * then impl' type selection menu
 *   - press space at menu to regret/terminate
 *       - recover inner to a legal place
 *   - show menu in gui
 *       - send string to qml. if empty string, close menu
 * then impl' map (pair) insert mode
 *   - menu provide types according to outer
 *       - array->any, object->pair, key->string
 *   - everything is fine grained, do autoxx later
 * stack gui effects
 */
void Doc::keyboard(char key)
{
    assert(modes.size() > 0);
    modes.top()->keyboard(key);
}

void Doc::registerRawRowsObserver(TokensObserver *ob)
{
    tokens.registerObserver(ob);
}

void Doc::push(Mode *mode)
{
    modes.emplace(mode);
    modes.top()->onPushed();
}

void Doc::pop()
{
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
        if (type == Ast::Type::OBJECT || type == Ast::Type::ARRAY) {
            outer = &focus;
            inner = 0;
            // TODO: change to general insert menu
            push(new MenuMode(*this));
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
    case Ast::Type::SCALAR:
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

void Doc::insert(Ast::Type type)
{
    Ast *a;

    switch (type) {
    case Ast::Type::PAIR:
        a = new MapAst(Ast::Type::PAIR);
        a->insert(0, new ScalarAst(Ast::Type::KEY, ""));
        a->insert(1, new ScalarAst(Ast::Type::SCALAR, ""));
        break;
    case Ast::Type::SCALAR:
        a = new ScalarAst(Ast::Type::SCALAR, "");
        break;
    default:
        qDebug() << "insert type: untreated outer type";
        break;
    }

    assert(a != nullptr);

    outer->insert(inner, a);
    tokens.insert(outer, inner);
}

void Doc::remove()
{
    if (outer->getType() != Ast::Type::ARRAY
            && outer->getType() != Ast::Type::OBJECT) {
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
            // re-insert to generate special empty text look
            tokens.remove(outer, inner);
            tokens.insert(outer, inner);
        }
    }
    tokens.light(&outer->at(inner));
}

