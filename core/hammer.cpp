#include "core/hammer.h"
#include <core/fleshtoken.h>
#include <core/bonetoken.h>
#include <core/soultoken.h>

#include <cassert>

Hammer::Hammer(Tokens &rawRows) :
    rawRows(rawRows)
{

}

void Hammer::write(const Ast &value)
{
    switch (value.getType()) {
    case Ast::Type::ROOT:
        write(value.at(0));
        break;
    case Ast::Type::ARRAY:
        writeArray(dynamic_cast<const ListAst&>(value));
        break;
    case Ast::Type::OBJECT:
        writeObject(dynamic_cast<const ListAst&>(value));
        break;
    case Ast::Type::SCALAR:
        writeScalar(dynamic_cast<const ScalarAst&>(value));
        break;
    default:
        throw -2;
        break;
    }
}

void Hammer::writeScalar(const ScalarAst &scalar)
{
    rawRows.insert(new FleshToken(&scalar));
    rawRows.newLine();
}

void Hammer::writeObject(const ListAst &object)
{
    rawRows.insert(new SoulToken(&object, Token::Role::BEGIN));

    rawRows.insert(new BoneToken(&object, "{"));
    rawRows.newLine();
    ++indentLevel;

    for (size_t i = 0; i < object.size(); i++) {
        const Ast &pair_ = object.at(i);
        assert(pair_.getType() == Ast::Type::PAIR);
        const MapAst &pair = dynamic_cast<const MapAst&>(pair_);

        indent(&pair);
        rawRows.insert(new SoulToken(&pair, Token::Role::BEGIN));
        const ScalarAst &key = dynamic_cast<const ScalarAst&>(pair.at(0));
        rawRows.insert(new FleshToken(&key));
        rawRows.insert(new BoneToken(&pair, ": "));
        write(pair.at(1));
        rawRows.insert(new SoulToken(&pair, Token::Role::END));
    }

    --indentLevel;
    indent(&object);
    rawRows.insert(new BoneToken(&object, "}"));
    rawRows.newLine();

    rawRows.insert(new SoulToken(&object, Token::Role::END));
}

void Hammer::writeArray(const ListAst &array)
{
    rawRows.insert(new SoulToken(&array, Token::Role::BEGIN));

    rawRows.insert(new BoneToken(&array, "["));
    rawRows.newLine();
    ++indentLevel;

    size_t size = array.size();
    for (size_t i = 0; i < size; i++) {
        indent(&array.at(i));
        write(array.at(i));
    }

    --indentLevel;
    indent(&array);
    rawRows.insert(new BoneToken(&array, "]"));
    rawRows.newLine();

    rawRows.insert(new SoulToken(&array, Token::Role::END));
}


void Hammer::indent(const Ast *master)
{
    if (indentLevel > 0) {
        Token *t = new BoneToken(master, std::string(indentLevel, '\t'));
        rawRows.insert(t);
    }
}

