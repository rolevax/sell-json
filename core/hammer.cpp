#include "core/hammer.h"
#include "core/tokens.h"
#include "core/fleshtoken.h"
#include "core/bonetoken.h"
#include "core/soultoken.h"

#include <cassert>

Hammer::Hammer(Tokens &tokens) :
    tokens(tokens)
{

}

void Hammer::write(const Ast &ast)
{
    switch (ast.getType()) {
    case Ast::Type::ROOT:
        write(ast.at(0));
        break;
    case Ast::Type::ARRAY:
        writeArray(dynamic_cast<const ListAst&>(ast));
        break;
    case Ast::Type::OBJECT:
        writeObject(dynamic_cast<const ListAst&>(ast));
        break;
    case Ast::Type::SCALAR:
        writeScalar(dynamic_cast<const ScalarAst&>(ast));
        break;
    default:
        throw -2;
        break;
    }
}

void Hammer::writeScalar(const ScalarAst &scalar)
{
    tokens.write(new FleshToken(&scalar));
    tokens.newLine();
}

void Hammer::writeObject(const ListAst &object)
{
    tokens.write(new SoulToken(&object, Token::Role::BEGIN));

    tokens.write(new BoneToken(&object, "{"));
    tokens.newLine();

    for (size_t i = 0; i < object.size(); i++) {
        const Ast &pair_ = object.at(i);
        assert(pair_.getType() == Ast::Type::PAIR);
        const MapAst &pair = dynamic_cast<const MapAst&>(pair_);

        indent(&pair);
        tokens.write(new SoulToken(&pair, Token::Role::BEGIN));
        const ScalarAst &key = dynamic_cast<const ScalarAst&>(pair.at(0));
        tokens.write(new FleshToken(&key));
        tokens.write(new BoneToken(&pair, ": "));
        write(pair.at(1));
        tokens.write(new SoulToken(&pair, Token::Role::END));
    }

    indent(&object);
    tokens.write(new BoneToken(&object, "}"));
    tokens.newLine();

    tokens.write(new SoulToken(&object, Token::Role::END));
}

void Hammer::writeArray(const ListAst &array)
{
    tokens.write(new SoulToken(&array, Token::Role::BEGIN));

    tokens.write(new BoneToken(&array, "["));
    tokens.newLine();

    size_t size = array.size();
    for (size_t i = 0; i < size; i++) {
        indent(&array.at(i));
        write(array.at(i));
    }

    indent(&array);
    tokens.write(new BoneToken(&array, "]"));
    tokens.newLine();

    tokens.write(new SoulToken(&array, Token::Role::END));
}

void Hammer::indent(const Ast *master)
{
    int level = 0;
    const Ast *a = &master->getParent();
    while (a->getType() != Ast::Type::ROOT) {
        ++level;
        a = &a->getParent();
    }

    if (level > 0) {
        Token *t = new BoneToken(master, std::string(level, '\t'));
        tokens.write(t);
    }
}

