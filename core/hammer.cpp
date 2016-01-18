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
    bool valueInPair = ast.getParent().getType() == Ast::Type::PAIR
            && ast.getParent().indexOf(&ast) == 1;
    //hitGeneral(ast, r, c, !valueInPair, !valueInPair);
    hitGeneral(ast, r, c, !valueInPair, false);
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

void Hammer::hitGeneral(const Ast &ast, size_t &r, size_t &c, bool tab, bool enter)
{
    switch (ast.getType()) {
    case Ast::Type::ROOT:
        hitGeneral(ast.at(0), r, c, tab, enter);
        break;
    case Ast::Type::ARRAY:
    case Ast::Type::OBJECT:
        hitList(static_cast<const ListAst&>(ast), r, c, tab, enter);
        break;
    case Ast::Type::STRING:
    case Ast::Type::NUMBER:
    case Ast::Type::KEY:
    case Ast::Type::KEYTAL:
        hitScalar(static_cast<const ScalarAst&>(ast), r, c, tab, enter);
        break;
    case Ast::Type::PAIR:
        hitPair(static_cast<const MapAst&>(ast), r, c);
        break;
    default:
        throw -2;
        break;
    }
}

void Hammer::hitScalar(const ScalarAst &scalar, size_t &r, size_t &c,
                       bool tab, bool enter)
{
    if (tab)
        indent(&scalar, r, c);

    bool needQuotes = scalar.getType() == Ast::Type::STRING
            || scalar.getType() == Ast::Type::KEY;

    write(new SoulToken(&scalar, Token::Role::BEGIN), r, c);
    if (needQuotes)
        write(new BoneToken(&scalar, "\""), r, c);
    write(new FleshToken(&scalar), r, c);
    if (needQuotes)
        write(new BoneToken(&scalar, "\""), r, c);
    write(new SoulToken(&scalar, Token::Role::END, enter), r, c);
}

void Hammer::hitList(const ListAst &list, size_t &r, size_t &c,
                     bool tab, bool enter)
{
    // either array or object
    bool isArray = list.getType() == Ast::Type::ARRAY;

    if (tab)
        indent(&list, r, c);

    write(new SoulToken(&list, Token::Role::BEGIN), r, c);

    size_t size = list.size();
    if (size > 0) {
        write(new BoneToken(&list, isArray ? "[" : "{", true), r, c);

        for (size_t i = 0; i < size; i++) {
            hitGeneral(list.at(i), r, c, true, false);
            write(new BoneToken(&list, i == size - 1 ? "" : ",", true), r, c);
        }

        indent(&list, r, c);
        write(new BoneToken(&list, isArray ? "]" : "}"), r, c);
    } else {
        write(new BoneToken(&list, isArray ? "[]" : "{}"), r, c);
    }

    write(new SoulToken(&list, Token::Role::END, enter), r, c);
}

void Hammer::hitPair(const MapAst &pair, size_t &r, size_t &c)
{
    indent(&pair, r, c);
    write(new SoulToken(&pair, Token::Role::BEGIN), r, c);

    hitGeneral(pair.at(0), r, c, true, false);
    write(new BoneToken(&pair, ": "), r, c);
    hitGeneral(pair.at(1), r, c, false, false);

    // pairs have no line break, since that's done by bone commas
    write(new SoulToken(&pair, Token::Role::END, false), r, c);
}

void Hammer::indent(const Ast *master, size_t &r, size_t &c)
{
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

