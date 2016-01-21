// SELL headers
#include "sell/core/doc.h"
#include "sell/mode/viewmode.h"
#include "sell/mode/menumode.h"
#include "sell/ast/astconverter.h"
#include "sell/gui/pdoc.h"

// RapidJSON headers
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"

// headers for file I/O
#include <fstream>
#include <sstream>

// utilities
#include <stdexcept>

// headers for debug uses
#include <QDebug>
#include <iostream>
#include <cstdlib>

Doc::Doc(PDoc *ob)
    : ob(ob)
{
    modes.emplace(new ViewMode(*this));
}

void Doc::load(const std::string &filename)
{
    if (modes.size() > 1)
        throw std::runtime_error("Load failed: modifying mode not popped");

    std::ifstream ifs(filename.c_str());
    std::stringstream ss;
    ss << ifs.rdbuf();
    ifs.close();
    std::string str(ss.str());
    const char *json = str.c_str();

    rapidjson::Document d;
    if (d.Parse(json).HasParseError()) {
        std::stringstream ss;
        ss << "Parse error: "
           << GetParseError_En(d.GetParseError());
        throw std::runtime_error(ss.str());
    }

    if (root.size() != 0) {
        outer = &root;
        inner = 0;
        remove();
    }

    AstConverter ac(root);
    ac.convert(d);

    tokens.insert(outer, inner);
    tokens.light(&outer->at(inner));
}

void Doc::save(const std::string &filename)
{
    if (modes.size() > 1)
        throw std::runtime_error("Save failed: modifying mode not popped");

    std::ofstream ofs(filename.c_str());
    ofs << tokens;
    ofs.close();
}

/**
 * @brief Accept a key stroke
 * Handle it with the mode on the top of the mode stack.
 */
void Doc::keyboard(char key)
{
    assert(modes.size() > 0);
    if (root.size() > 0)
        modes.top()->keyboard(key);
    else
        modes.top()->emptyKeyboard(key);
}

void Doc::registerTokensObserver(TokensObserver *ob)
{
    tokens.registerObserver(ob);
}

/**
 * @brief Push 'mode' onto the mode stack,
 * Trigger the 'onPushed' callback of 'mode'
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
 * @brief Pop the top of the mode stack,
 * @param nextPush the next mode to push
 * Call the 'onPopped' of that mode.
 * Then if nextPush is not null, push that.
 * Else call 'onResume' of the new top mode.
 * Afterwards that mode object will be destructed.
 */
void Doc::pop(Mode *nextPush)
{
    assert(modes.size() > 1); // bottom view mode reserved

    if (ob != nullptr)
        ob->observePop();

    std::unique_ptr<Mode> popped = std::move(modes.top());
    modes.pop();
    popped->onPopped();

    if (nextPush != nullptr)
        push(nextPush);
    else
        modes.top()->onResume();
}

Ast::Type Doc::outerType()
{
    return outer->getType();
}

Ast::Type Doc::innerType()
{
    return outer->at(inner).getType();
}

void Doc::cursorIn()
{
    outer = &outer->at(inner);
    inner = 0;
}

void Doc::cursorForward()
{
    ++inner;
}

void Doc::fallIn()
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

void Doc::digOut()
{
    if (outer == &root) {
        return;
    }

    Ast *nextOuter = &outer->getParent();
    inner = nextOuter->indexOf(outer);
    tokens.light(outer);
    outer = nextOuter;
}

void Doc::sibling(int step)
{
    size_t size = outer->size();
    inner = (ssize_t(inner + size) + step) % size;
    tokens.light(&outer->at(inner));
}

void Doc::jackKick(bool down)
{
    tokens.jackKick(outer, inner, down);
}

void Doc::hackLead(bool right)
{
    if (outerType() == Ast::Type::PAIR && inner == 0)
        sibling(right ? +1 : -1);
}

/**
 * @brief Create an empty subnode
 * @param type the type of the new node
 * Create a new subnode inside 'outer' at 'inner',
 * with specified type 'type'.
 * The value of the new node is "as empty as possible".
 * TODO number and keytal, is "", makes it inconsistent, might be a bad design
 */
void Doc::insert(Ast::Type type)
{
    assert(inner <= outer->size());

    Ast *a = newTree(type);

    outer->insert(inner, a);
    tokens.insert(outer, inner);
}

std::unique_ptr<Ast> Doc::remove()
{
    assert(inner < outer->size());

    tokens.remove(outer, inner);
    std::unique_ptr<Ast> ret = outer->remove(inner);
    if (outer->getType() == Ast::Type::ROOT) {
        // nothing to do yet, just leave it here
    } else if (Ast::isList(*outer)) {
        if (inner == outer->size()) {
            if (inner > 0) {
                --inner;
            } else { // outer became empty
                digOut();
                // re-insert to generate special empty look
                tokens.remove(outer, inner);
                tokens.insert(outer, inner);
            }
        }
        tokens.light(&outer->at(inner));
    }

    return ret;
}

void Doc::change(Ast::Type type)
{
    assert(inner < outer->size());

    if (!Ast::isChangeable(outer->at(inner))) {
        qDebug() << "unchangable inner";
        return;
    }

    tokens.remove(outer, inner);
    outer->change(inner, newTree(type));
    tokens.insert(outer, inner);
    tokens.light(&outer->at(inner));
}

void Doc::nest(Ast::Type type)
{
    assert(inner < outer->size());
    // a kind of fucking design.
    assert(type == Ast::Type::ARRAY);

    // nestable only if changeable
    if (!Ast::isChangeable(outer->at(inner))) {
        qDebug() << "unnestable inner";
        return;
    }

    tokens.remove(outer, inner);
    outer->nest(inner, newTree(type));
    tokens.insert(outer, inner);
    tokens.light(&outer->at(inner));
}

void Doc::scalarAppend(const char *str)
{
    ScalarAst &scalar = getScalar();
    while ('\0' != *str)
        scalar.append(*str++);
    tokens.updateScalar(outer, inner);
}

void Doc::scalarAppend(char c)
{
    getScalar().append(c);
    tokens.updateScalar(outer, inner);
}

void Doc::scalarClear()
{
    getScalar().clear();
    tokens.updateScalar(outer, inner);
}

void Doc::light()
{
    // TODO: apply this in this cpp
    tokens.light(&outer->at(inner));
}

void Doc::setHotLight(bool b)
{
    // TODO: move all language-aware things to ast/* or hammer
    if (b) {
        ssize_t back = innerType() == Ast::Type::KEY
                || innerType() == Ast::Type::STRING ? 1 : 0;
        tokens.setHotLight(back);
    } else {
        tokens.setHotLight(-1);
    }
}

void Doc::toggleTension(bool b)
{
    if (ob != nullptr)
        ob->observeTension(b);
}

Ast *Doc::newTree(Ast::Type type)
{
    Ast *a = nullptr;

    switch (type) {
    case Ast::Type::PAIR:
        a = new MapAst(Ast::Type::PAIR);
        a->insert(0, new ScalarAst(Ast::Type::KEY, ""));
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
        throw "newTree: untreated type";
    }

    return a;
}

ScalarAst &Doc::getScalar()
{
    assert(Ast::isScalar(outer->at(inner)));
    return static_cast<ScalarAst&>(outer->at(inner));
}

