#include "sell/core/doc.h"
#include "sell/ast/astconverter.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

#include <chrono>
#include <QDebug>
#include <iostream>

Doc::Doc()
{
    modes.push(Mode::VIEW);
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

void Doc::keyboard(char key)
{
    assert(modes.size() > 0);
    switch (modes.top()) {
    case Mode::VIEW:
        keyView(key);
        break;
    case Mode::TYPE_MENU:
        keyMenu(key);
        break;
    case Mode::INPUT_STRING:
        /*
         * TODO XXX
         * make modes objects, because modes are too diversely stateful
         *   - Modes has full control to Doc. just for modulization.
         *   - combine pop();push() into one pop()
         *      - MenuMode::leave(nextMode) do that.
         *        pop self in doc, and push next (kind of 'delete this')
         *        (since dangerous for memory, can Mode::leave(),
         *        don't trust subclasses)
         *   - ViewMode has bookmarked inner, outer;
         *     MenuMode has append:bool
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
        keyInputString(key);
        break;
    case Mode::INPUT_NUMBER:
        keyInputNumber(key);
        break;
    default:
        throw 999; // TODO search all 'throw'
    }
}

void Doc::registerRawRowsObserver(TokensObserver *ob)
{
    tokens.registerObserver(ob);
}

void Doc::push(Mode mode)
{
    modes.push(mode);
    switch (mode) {
    case Mode::VIEW:
        break;
    case Mode::TYPE_MENU:
        break;
    case Mode::INPUT_STRING:
        insert(Ast::Type::SCALAR);
        tokens.light(&outer->at(inner));
        tokens.setHotLight(true);
        break;
    case Mode::INPUT_NUMBER:
        insert(Ast::Type::SCALAR);
        tokens.light(&outer->at(inner));
        tokens.setHotLight(true);
        break;
    }
}

void Doc::pop()
{
    Mode poped = modes.top();
    modes.pop();

    switch (poped) {
    case Mode::VIEW:
        break;
    case Mode::TYPE_MENU:
        break;
    case Mode::INPUT_STRING:
    case Mode::INPUT_NUMBER:
        tokens.setHotLight(false);
        break;
    default:
        break;
    }
}

void Doc::keyView(char key)
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
        if (outer->getType() == Ast::Type::OBJECT
                || outer->getType() == Ast::Type::ARRAY)
            ++inner;
        /* fall-through */
    case 'i':
        if (outer->getType() == Ast::Type::OBJECT
                || outer->getType() == Ast::Type::ARRAY)
            push(Mode::TYPE_MENU);
        break;
    case 'x':
        remove();
        break;
    default:
        qDebug() << "Doc: unsupported key in view mode";
        break;
    }
}

void Doc::keyInputString(char key)
{
    switch (key) {
    case ' ':
        pop();
        break;
    default:
        assert(outer->at(inner).getType() == Ast::Type::SCALAR);
        ScalarAst &scalar = static_cast<ScalarAst&>(outer->at(inner));
        scalar.append(key);
        tokens.updateScalar(outer, inner);
        tokens.light(&outer->at(inner));
        break;
    }
}

void Doc::keyInputNumber(char key)
{
    assert(outer->at(inner).getType() == Ast::Type::SCALAR);

    if (' ' == key) {
        pop();
        return;
    }

    char input;
    if ('0' <= key && key <= '9')
        input = key;
    else if ('u' == key)
        input = '4';
    else if ('i' == key)
        input = '5';
    else if ('o' == key)
        input = '6';
    else if ('j' == key)
        input = '1';
    else if ('k' == key)
        input = '2';
    else if ('l' == key)
        input = '3';
    else if ('m' == key)
        input = '0';
    else
        return;

    ScalarAst &scalar = static_cast<ScalarAst&>(outer->at(inner));
    scalar.append(input);
    tokens.updateScalar(outer, inner);
    tokens.light(&outer->at(inner));
}

void Doc::keyMenu(char key)
{
    switch (key) {
    case ' ':
        pop();
        break;
    case 's':
        pop();
        push(Mode::INPUT_STRING);
        break;
    case 'n':
        pop();
        push(Mode::INPUT_NUMBER);
        break;
    default:
        break;
    }
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
            push(Mode::INPUT_STRING);
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

