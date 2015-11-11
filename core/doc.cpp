#include "core/doc.h"
#include "ast/astconverter.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

#include <QDebug>
#include <iostream>

Doc::Doc()
{
    modes.push(Mode::VIEW);
}

void Doc::load()
{
    assert(!root.isPresent());
    const char *json = "[{"
                       "\"name\": \"you\","
                       "\"age\": 18,"
                       "\"fucked\": true"
                       "},"
                       "123,"
                       "[34, 79, 90],"
                       "[],"
                       "9.75"
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
    case Mode::INPUT_SCALAR:
        /* TODO XXX
         * do single scalar insert first.
         *   - no gui input behaviors
         *   - focus a text field on the scalar
         * then impl' list insert mode
         * then impl' type selection menu
         * stack gui effects
         */
        keyInput(key);
        break;
    default:
        throw 999; // TODO search all 'throw'
    }
}

void Doc::registerRawRowsObserver(TokensObserver *ob)
{
    tokens.registerObserver(ob);
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
    case 'i':
        modes.push(Mode::INPUT_SCALAR);
        insert();
        break;
    case 'x':
        remove();
        break;
    default:
        qDebug() << "Doc: unsupported key in view mode";
        break;
    }
}

void Doc::keyInput(char key)
{
    (void) key;
    switch (key) {
    case ' ':
        modes.pop();
        break;
    default:
        assert(outer->at(inner).getType() == Ast::Type::SCALAR);
        ScalarAst &scalar = static_cast<ScalarAst&>(outer->at(inner));
        scalar.append(key);
        tokens.remove(outer, inner);
        tokens.insert(outer, inner);
        break;
    }
}

void Doc::fuckIn()
{
    if (inner == outer->size()) {
        qDebug() << "end not fuckable";
        return;
    }

    if (outer->at(inner).size() == 0) {
        qDebug() << "empty not fuckable";
        return;
    }

    Ast::Type type = outer->at(inner).getType();
    switch (type) {
    case Ast::Type::ARRAY:
    case Ast::Type::OBJECT:
    case Ast::Type::PAIR:
    case Ast::Type::ROOT:
        outer = &outer->at(inner);
        inner = 0;
        tokens.light(&outer->at(inner));
        break;
    case Ast::Type::KEY:
    case Ast::Type::SCALAR:
        qDebug() << "unfuckable inner node";
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
    for (size_t i = 0; i < nextOuter->size(); i++)
        if (&nextOuter->at(i) == outer)
            inner = i;
    tokens.light(outer);
    outer = nextOuter;
}

void Doc::jackKick(int step)
{
    size_t nextInner = ssize_t(inner) + step;
    if (nextInner >= outer->size()) {
        qDebug() << "jackKick: out of range: " << nextInner;
        return;
    }
    inner = nextInner;
    tokens.light(&outer->at(inner));
}

void Doc::insert()
{
    if (outer->getType() != Ast::Type::ARRAY)
        return; // TODO: also enable Object
    // test
    std::unique_ptr<Ast> a(new ScalarAst(Ast::Type::SCALAR, "i"));
    outer->insert(inner, a);
    tokens.insert(outer, inner);
    // TODO: should move focus only in outer = list
    //++inner;
    tokens.light(&outer->at(inner));
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
        if (inner > 0)
            --inner;
        else
            damnOut(); // TODO: remove child of root case
    }
    tokens.light(&outer->at(inner));
}

