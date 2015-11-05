#include "core/tokens.h"
#include <QDebug>
#include <iostream>

#include <cassert>

Tokens::Tokens() :
    hammer(*this)
{
    rows.emplace_back();
}

Tokens::Region Tokens::locate(const Ast *tar)
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
                if (role == Token::Role::FLESH) {
                    res.br = rit - rows.begin();
                    res.bc = cit - rit->begin();
                    res.er = res.br;
                    res.ec = res.bc;
                    found = true;
                    break;
                } else if (role == Token::Role::BEGIN) {
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

void Tokens::suck()
{
    if (r == rows.size() || c == rows[r].size())
        return;
    // TODO
        /*
         * when press 'i' to insert, use this suck() to locate the
         * first token behind the previous token which does not
         * belong to the previous token (next to end/flesh)
         * skip separator token of parent AST if any.
         *   ---> just for skipping leading tabs
         *         ---> can't include tabs behind begin, because of flesh
         */
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

void Tokens::newLine(size_t r, size_t c)
{
    bool chop = r < rows.size() && c < rows[r].size();
    rows.emplace(rows.begin() + r + 1);
    for (auto ob : observers)
        ob->observeNewLine(r, c);

    if (chop) {
        // TODO re-design newline() and insert.
        // (for the total system, including qml)
        // row insertion should be explicity reported
        // p.s. press enter is the only way to create new row.
    }
}

void Tokens::write(Token *token, size_t r, size_t c)
{
    assert(r < rows.size() && c <= rows[r].size());
    std::vector<std::unique_ptr<Token>> &row = rows[r];
    row.emplace(row.begin() + c, token);
    for (auto ob : observers)
        ob->observeInsert(*token, r, c);
}

void Tokens::insert(const Ast *outer, size_t inner)
{
    assert(outer->size() > 0);

    if (outer->getType() == Ast::Type::ROOT) {
        hammer.write(*outer, 0, 0);
    } else if (outer->size() == 1) {
        // delete old outer token
        // insert new outer token to outer's outer (until root)
    } else if (inner == 0) {
        Region prevHead = locate(&outer->at(1));
        // suck prevHead
        // insert br, bc
    } else {
        Region ass = locate(&outer->at(inner - 1));
        // insert ass.er, ass.ec + 1
    }
}

void Tokens::erase(size_t br, size_t bc, size_t er, size_t ec)
{
    assert(br < rows.size() && bc < rows[br].size());
    assert(er < rows.size() && ec < rows[er].size());
    assert(br <= er);

    if (br == er) {
        auto it = rows[br].begin();
        rows[br].erase(it + bc, it + ec + 1);
        if (rows[br].empty())
            rows.erase(rows.begin() + br);
    } else {
        auto endIt = rows[er].begin();
        rows[er].erase(endIt, endIt + ec + 1);
        if (rows[er].empty())
            rows.erase(rows.begin() + er);

        if (br + 1 < er) // more than 2 rows
            rows.erase(rows.begin() + br + 1, rows.begin() + er);

        rows[br].erase(rows[br].begin() + bc, rows[br].end());
        if (rows[br].empty())
            rows.erase(rows.begin() + br);
    }
}

void Tokens::remove()
{
    assert(r < rows.size() && c < rows[r].size());
    Token::Role role = rows[r][c]->getRole();
    assert(role == Token::Role::BEGIN || role == Token::Role::FLESH);

    const Ast *target = rows[r][c]->getAst();
    // TODO: optmz
    int br = r, bc = c, er = -1, ec;
    for (auto rit = rows.begin(); rit != rows.end(); ++rit) {
        for (auto cit = rit->begin(); cit != rit->end(); ++cit) {
            const Ast *ast = (*cit)->getAst();
            if (ast == target && (*cit)->getRole() == Token::Role::END) {
                er = rit - rows.begin();
                ec = cit - rit->begin();
                break;
            }
        }
    }
    assert(er != -1);

    // check leading tab token
    if (bc - 1 >= 0 && rows[br][bc - 1]->getAst() == target)
        --bc;
    erase(br, bc, er, ec);
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


