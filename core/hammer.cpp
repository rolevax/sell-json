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
    bool newLine = tokens.write(token, r, c);
    if (newLine) {
        ++r;
        c = 0;
    } else {
        ++c;
    }
}

void Hammer::hitGeneral(const Ast &ast, size_t &r, size_t &c)
{
    if (ast.getParent().getType() == Ast::Type::PAIR
            && ast.getParent().indexOf(&ast) == 1)
        absorbOneIndent = true; // 'value' inside 'pair'

    // FIXIT XXX: a newline of 'value' is included in end of 'pair'
    // sol: parameterize 'bool newline', decide when call

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
    case Ast::Type::STRING:
    case Ast::Type::NUMBER:
    case Ast::Type::KEY:
    case Ast::Type::KEYTAL:
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
    write(new SoulToken(&scalar, Token::Role::END, true), r, c);
}

void Hammer::hitObject(const ListAst &object, size_t &r, size_t &c)
{
    indent(&object, r, c);
    write(new SoulToken(&object, Token::Role::BEGIN), r, c);

    size_t size = object.size();
    if (size > 0) {
        write(new BoneToken(&object, "{", true), r, c);

        for (size_t i = 0; i < object.size(); i++) {
            hitGeneral(object.at(i), r, c); // including hit pair case
        }

        indent(&object, r, c);
        write(new BoneToken(&object, "}"), r, c);
    } else {
        write(new BoneToken(&object, "{}"), r, c);
    }

    write(new SoulToken(&object, Token::Role::END, true), r, c);
}

void Hammer::hitArray(const ListAst &array, size_t &r, size_t &c)
{
    indent(&array, r, c);
    write(new SoulToken(&array, Token::Role::BEGIN), r, c);

    size_t size = array.size();
    if (size > 0) {
        write(new BoneToken(&array, "[", true), r, c);

        for (size_t i = 0; i < size; i++) {
            hitGeneral(array.at(i), r, c);
        }

        indent(&array, r, c);
        write(new BoneToken(&array, "]"), r, c);
    } else {
        write(new BoneToken(&array, "[]"), r, c);
    }

    write(new SoulToken(&array, Token::Role::END, true), r, c);
}

void Hammer::hitPair(const MapAst &pair, size_t &r, size_t &c)
{
    indent(&pair, r, c);
    write(new SoulToken(&pair, Token::Role::BEGIN), r, c);

    const ScalarAst *key = static_cast<const ScalarAst*>(&pair.at(0));
    write(new SoulToken(key, Token::Role::BEGIN), r, c);
    write(new FleshToken(key), r, c);
    write(new SoulToken(key, Token::Role::END), r, c);

    write(new BoneToken(&pair, ": "), r, c);

    hitGeneral(pair.at(1), r, c);

    write(new SoulToken(&pair, Token::Role::END), r, c);
}

void Hammer::indent(const Ast *master, size_t &r, size_t &c)
{
    if (absorbOneIndent) {
        absorbOneIndent = false;
        return;
    }

    if (master->getType() == Ast::Type::KEY)
        return; // key's indent is done by pair

    int level = 0;
    const Ast *a = &master->getParent();

    while (a->getType() != Ast::Type::ROOT) {
        if (a->getType() != Ast::Type::PAIR)
            ++level;
        a = &a->getParent();
    }

    if (level > 0) {
        Token *t = new BoneToken(master, std::string(level * 4, ' '));
        write(t, r, c);
    }
}

