#include "sell/core/hammer.h"
#include "sell/core/tokens.h"
#include "sell/core/fleshtoken.h"
#include "sell/core/bonetoken.h"
#include "sell/core/soultoken.h"

#include <cassert>

Hammer::Hammer(Tokens &tokens) :
    tokens(tokens)
{

}

void Hammer::hit(const Ast &ast, size_t r, size_t c)
{
    hitGeneral(ast, r, c);
}

void Hammer::write(Token *token, size_t &r, size_t &c)
{
    tokens.write(token, r, c);
    ++c;
}

void Hammer::newLine(size_t &r, size_t &c)
{
    tokens.newLine(r, c);
    ++r;
    c = 0;
}

void Hammer::hitGeneral(const Ast &ast, size_t &r, size_t &c)
{
    switch (ast.getType()) {
    case Ast::Type::ROOT:
        hitGeneral(ast.at(0), r, c);
        break;
    case Ast::Type::ARRAY:
        hitArray(static_cast<const ListAst&>(ast), r, c);
        break;
    case Ast::Type::OBJECT:
        hitObject(static_cast<const ListAst&>(ast), r, c);
        break;
    case Ast::Type::SCALAR:
        hitScalar(static_cast<const ScalarAst&>(ast), r, c);
        break;
    case Ast::Type::PAIR:
        hitPair(static_cast<const MapAst&>(ast), r, c);
        break;
    default:
        throw -2;
        break;
    }
}

void Hammer::hitScalar(const ScalarAst &scalar, size_t &r, size_t &c)
{
    indent(&scalar, r, c);
    write(new SoulToken(&scalar, Token::Role::BEGIN), r, c);
    write(new FleshToken(&scalar), r, c);
    newLine(r, c);
    write(new SoulToken(&scalar, Token::Role::END), r, c);
}

void Hammer::hitObject(const ListAst &object, size_t &r, size_t &c)
{
    indent(&object, r, c);
    write(new SoulToken(&object, Token::Role::BEGIN), r, c);

    size_t size = object.size();
    if (size > 0) {
        write(new BoneToken(&object, "{"), r, c);
        newLine(r, c);

        for (size_t i = 0; i < object.size(); i++) {
            const Ast &pair_ = object.at(i);
            assert(pair_.getType() == Ast::Type::PAIR);
            const MapAst &pair = dynamic_cast<const MapAst&>(pair_);
            hitPair(pair, r, c);
        }

        indent(&object, r, c);
        write(new BoneToken(&object, "}"), r, c);
    } else {
        write(new BoneToken(&object, "{}"), r, c);
    }
    newLine(r, c);

    write(new SoulToken(&object, Token::Role::END), r, c);
}

void Hammer::hitArray(const ListAst &array, size_t &r, size_t &c)
{
    indent(&array, r, c);
    write(new SoulToken(&array, Token::Role::BEGIN), r, c);

    size_t size = array.size();
    if (size > 0) {
        write(new BoneToken(&array, "["), r, c);
        newLine(r, c);

        for (size_t i = 0; i < size; i++) {
            hitGeneral(array.at(i), r, c);
        }

        indent(&array, r, c);
        write(new BoneToken(&array, "]"), r, c);
    } else {
        write(new BoneToken(&array, "[]"), r, c);
    }
    newLine(r, c);

    write(new SoulToken(&array, Token::Role::END), r, c);
}

void Hammer::hitPair(const MapAst &pair, size_t &r, size_t &c)
{
    indent(&pair, r, c);
    write(new SoulToken(&pair, Token::Role::BEGIN), r, c);

    const ScalarAst *key = dynamic_cast<const ScalarAst*>(&pair.at(0));
    write(new SoulToken(key, Token::Role::BEGIN), r, c);
    write(new FleshToken(key), r, c);
    write(new SoulToken(key, Token::Role::END), r, c);

    write(new BoneToken(&pair, ": "), r, c);

    hitGeneral(pair.at(1), r, c);

    write(new SoulToken(&pair, Token::Role::END), r, c);
}

void Hammer::indent(const Ast *master, size_t &r, size_t &c)
{
    if (master->getType() == Ast::Type::KEY)
        return;

    int level = 0;
    const Ast *a = &master->getParent();
    if (a->getType() == Ast::Type::PAIR)
        return;

    while (a->getType() != Ast::Type::ROOT) {
        if (a->getType() != Ast::Type::PAIR)
            ++level;
        a = &a->getParent();
    }

    if (level > 0) {
        Token *t = new BoneToken(master, std::string(level, '\t'));
        write(t, r, c);
    }
}

