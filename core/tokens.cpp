#include "core/tokens.h"
#include <QDebug>
#include <iostream>

#include <cassert>

Tokens::Tokens() :
    hammer(*this)
{
    rows.emplace_back();
}

void Tokens::setHotLight(bool b)
{
    for (auto ob : observers)
        ob->observeSetHotLight(b);
}

void Tokens::light(const Ast *inner)
{
    Region in = locate(inner);
    Ast *outer = &inner->getParent();
    if (outer->getType() == Ast::Type::ROOT) {
        for (auto ob : observers)
            ob->observeLight(0, 0, 0, 0, in.br, in.bc, in.er, in.ec);
    } else {
        Region out = locate(outer);
        for (auto ob : observers)
            ob->observeLight(out.br, out.bc, out.er, out.ec,
                             in.br, in.bc, in.er, in.ec);
    }
}

void Tokens::insert(const Ast *outer, size_t inner)
{
    assert(outer->size() > 0 && inner < outer->size());

    if (outer->getType() == Ast::Type::ROOT) { // outer is root
        hammer.write(*outer, 0, 0);
    } else if (outer->size() == 1) { // was empty before insertion
        Ast *parent = &outer->getParent();
        size_t outerIndex = parent->indexOf(outer);
        remove(parent, outerIndex);
        insert(parent, outerIndex);
        /* recursion terminates when outer is root */
    } else if (inner == 0) { // at very beginning
        Region prevHead = locate(&outer->at(1));
        suck(prevHead);
        hammer.write(outer->at(inner), prevHead.br, prevHead.bc);
    } else { // internal or very end
        Region ass = locate(&outer->at(inner - 1));
        hammer.write(outer->at(inner), ass.er, ass.ec + 1);
    }
}

void Tokens::remove(const Ast *outer, size_t inner)
{
    assert(inner < outer->size());

    Region r = locate(&outer->at(inner));
    suck(r);
    erase(r);
}

void Tokens::updateScalar(const Ast *outer, size_t inner)
{
    assert(outer->at(inner).getType() == Ast::Type::SCALAR);
    Region r = locate(&outer->at(inner));
    updateFlesh(r);
}

void Tokens::print()
{
    std::cout << "Print from rawRows:" << std::endl;
    for (const auto &row : rows) {
        for (const auto &t : row)
            std::cout << t->getText();
        std::cout << std::endl;
    }
    std::cout << "End of rawRows print. Last \\n is external" << std::endl;
}

void Tokens::registerObserver(TokensObserver *ob)
{
    observers.push_back(ob);
}

Region Tokens::locate(const Ast *tar)
{
    if (tar->getType() == Ast::Type::ROOT)
        return { 0, 0, 0, 0 };

    Region res;
    bool found = false;

    for (auto rit = rows.begin(); rit != rows.end(); ++rit) {
        for (auto cit = rit->begin(); cit != rit->end(); ++cit) {
            const Ast *a = (*cit)->getAst();
            if (a == tar) {
                Token::Role role = (*cit)->getRole();
                if (role == Token::Role::BEGIN) {
                    res.br = rit - rows.begin();
                    res.bc = cit - rit->begin();
                } else if (role == Token::Role::END) {
                    res.er = rit - rows.begin();
                    res.ec = cit - rit->begin();
                    found = true;
                    break;
                }
            }
        }
    }

    assert(found);
    return res;
}

void Tokens::suck(Region &r)
{
    assert(r.br < rows.size() && r.bc < rows[r.br].size());
    Token::Role role = rows[r.br][r.bc]->getRole();
    if (role == Token::Role::BEGIN) {
        const Ast *ast = rows[r.br][r.bc]->getAst();
        if (r.bc > 0 && rows[r.br][r.bc - 1]->getAst() == ast)
            --r.bc; // skip one tab
    }
}

void Tokens::write(Token *token, size_t r, size_t c)
{
    assert(r < rows.size() && c <= rows[r].size());
    std::vector<std::unique_ptr<Token>> &row = rows[r];
    row.emplace(row.begin() + c, token);
    for (auto ob : observers)
        ob->observeWrite(*token, r, c);
}

void Tokens::erase(const Region &r)
{
    assert(r.br < rows.size() && r.bc < rows[r.br].size());
    assert(r.er < rows.size() && r.ec < rows[r.er].size());
    assert(r.br <= r.er);

    if (r.br == r.er) {
        auto it = rows[r.br].begin();
        rows[r.br].erase(it + r.bc, it + r.ec + 1);
    } else {
        // remove inside the last line
        auto endIt = rows[r.er].begin();
        rows[r.er].erase(endIt, endIt + r.ec + 1);
        if (rows[r.er].empty())
            rows.erase(rows.begin() + r.er);

        if (r.br + 1 < r.er) // remove internal lines
            rows.erase(rows.begin() + r.br + 1, rows.begin() + r.er);

        // remove inside the first line
        rows[r.br].erase(rows[r.br].begin() + r.bc, rows[r.br].end());

        if (r.br + 1 < rows.size())
            mergeLine(r.br + 1);
    }

    for (auto ob : observers)
        ob->observeErase(r);
}

void Tokens::updateFlesh(const Region &r)
{
    Token &t = *rows[r.br][r.bc + 1];
    assert(t.getRole() == Token::Role::FLESH);
    for (auto ob : observers)
        ob->observeUpdateFlesh(r.br, r.bc + 1, t);
}

/*
 * observer should also do the move-rest-of-line job
 * should do that with remove + re-insert to reuse code?
 */
void Tokens::newLine(size_t r, size_t c)
{
    assert(r < rows.size() && c <= rows[r].size());
    rows.emplace(rows.begin() + r + 1);
    auto &newRow = rows[r + 1];
    newRow.insert(newRow.end(),
                  std::make_move_iterator(rows[r].begin() + c),
                  std::make_move_iterator(rows[r].end()));
    rows[r].erase(rows[r].begin() + c, rows[r].end());

    for (auto ob : observers)
        ob->observeNewLine(r, c);
}

void Tokens::mergeLine(size_t r)
{
    assert(r > 0 && r < rows.size());
    auto &prevRow = rows[r - 1];
    prevRow.insert(prevRow.end(),
                   std::make_move_iterator(rows[r].begin()),
                   std::make_move_iterator(rows[r].end()));
    rows.erase(rows.begin() + r);
}


