#include "core/doc.h"
#include "ast/astconverter.h"
#include "mode/viewmode.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

#include <QDebug>
#include <iostream>

Doc::Doc() :
    modeStack(new std::stack<std::shared_ptr<Mode>>)
{
    std::shared_ptr<Mode> baseMode{new ViewMode};
    this->modeStack->push(baseMode);
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
//    this->modeStack->top()->execute(key);
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
        insert();
        break;
    default:
        qDebug() << "Doc: unsupported key in curront mode";
        break;
    }
}

void Doc::registerRawRowsObserver(TokensObserver *ob)
{
    tokens.registerObserver(ob);
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
    if (nextInner >= outer->size()) { // TODO: pointtoend
        qDebug() << "jackKick: out of range: " << nextInner;
        return;
    }
    inner = nextInner;
    // TODO: point to end cases
    tokens.light(&outer->at(inner));
}

void Doc::insert()
{
    // test
    std::unique_ptr<Ast> a(new ScalarAst(Ast::Type::SCALAR, "\"haha\""));
    outer->insert(inner, a);
    tokens.insert(outer, inner);
    ++inner;
    tokens.light(&outer->at(inner));
}

