#include "core/doc.h"
#include "ast/astconverter.h"
#include "mode/viewmode.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <QDebug>

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
                       "}]";
    rapidjson::Document d;
    d.Parse(json);
    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> pw(sb);
    d.Accept(pw);
    qDebug() << sb.GetString();

    AstConverter ac(root);
    ac.convert(d);

    // test
    std::unique_ptr<Ast> a(new ScalarAst(Ast::Type::SCALAR, "\"haha\""));
    root.at(0).insert(1, a);

    RawRowsWriter writer(rawRows);
    writer.write(root);
    rawRows.seek(&root.at(0));
    rawRows.light();
//    rawRows.print();
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
    default:
        qDebug() << "Doc: unsupported key in curront mode";
        break;
    }
}

void Doc::registerRawRowsObserver(RawRowsObserver *ob)
{
    rawRows.registerObserver(ob);
}

void Doc::fuckIn()
{
    if (inner == outer->size()) {
        qDebug() << "end not fuckable";
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
        if (outer->size() > 0) { // TODO: point to end cases
            rawRows.seek(&outer->at(inner));
            rawRows.light();
        }
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
    rawRows.seek(outer);
    rawRows.light();
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
    rawRows.seek(&outer->at(inner));
    rawRows.light();
}

