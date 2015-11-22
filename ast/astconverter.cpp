#include "sell/ast/astconverter.h"
#include "sell/ast/scalarast.h"
#include "sell/ast/listast.h"
#include "sell/ast/mapast.h"

#include <qdebug.h>

AstConverter::AstConverter(RootAst &root) :
    root(root)
{

}

void AstConverter::convert(const rapidjson::Document &d)
{
    d.Accept(*this);
}

bool AstConverter::Null()
{
    convertScalar("null");
    return true;
}

bool AstConverter::Bool(bool b)
{
    convertScalar(b ? "true" : "false");
    return true;
}

bool AstConverter::Int(int i)
{
    convertScalar(std::to_string(i));
    return true;
}

bool AstConverter::Uint(unsigned i)
{
    convertScalar(std::to_string(i));
    return true;
}

bool AstConverter::Int64(int64_t i)
{
    convertScalar(std::to_string(i));
    return true;
}

bool AstConverter::Uint64(uint64_t i)
{
    convertScalar(std::to_string(i));
    return true;
}

bool AstConverter::Double(double d)
{
    convertScalar(std::to_string(d));
    return true;
}

bool AstConverter::String(const AstConverter::Ch *str,
                           rapidjson::SizeType length, bool copy)
{
    (void) copy;
    (void) length;
    convertScalar(str);
    return true;
}

bool AstConverter::StartObject()
{
    Ast *obj = new ListAst(Ast::Type::OBJECT);
    outer->insert(inner, obj);
    outer = &outer->at(inner);
    inner = 0;

    return true;
}

bool AstConverter::Key(const AstConverter::Ch *str,
                        rapidjson::SizeType length, bool copy)
{
    (void) copy;
    (void) length;

    Ast *map = new MapAst(Ast::Type::PAIR);
    Ast *key = new ScalarAst(Ast::Type::KEY, str);
    map->insert(0, key);

    assert(outer->getType() == Ast::Type::OBJECT);
    outer->insert(inner, map);
    outer = &outer->at(inner); // outer = pair
    inner = 1; // inner = value

    return true;
}

bool AstConverter::EndObject(rapidjson::SizeType memberCount)
{
    (void) memberCount;

    outer = &outer->getParent();
    inner = outer->size();

    return true;
}

bool AstConverter::StartArray()
{
    Ast *ast = new ListAst(Ast::Type::ARRAY);
    outer->insert(inner, ast);
    outer = &outer->at(inner);
    inner = 0;

    return true;
}

bool AstConverter::EndArray(rapidjson::SizeType elementCount)
{
    (void) elementCount;

    outer = &outer->getParent();
    inner = outer->size();

    return true;
}

void AstConverter::convertScalar(const std::string &text)
{
    Ast *scalar = new ScalarAst(Ast::Type::SCALAR, text);
    outer->insert(inner, scalar);
    if (outer->getType() == Ast::Type::ARRAY) {
        ++inner;
    } else if (outer->getType() == Ast::Type::PAIR) {
        outer = &outer->getParent();
        assert(outer->getType() == Ast::Type::OBJECT);
        inner = outer->size();
    }
}

