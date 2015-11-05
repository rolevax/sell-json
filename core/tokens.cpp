#include "core/tokens.h"
#include <QDebug>
#include <iostream>

#include <cassert>

Tokens::Tokens() :
    hammer(*this)
{
    rows.emplace_back();
}

void Tokens::seek(ssize_t nextR, ssize_t nextC)
{
    if (nextR >= 0) {
        assert(size_t(nextR) <= rows.size());
        r = nextR;
    }

    if (nextC >= 0) {
        assert(nextC == 0 ||
               (r < rows.size() && size_t(nextC) <= rows[r].size()));
        c = nextC;
    }

    for (auto ob : observers)
        ob->observeCoord(r, c);
}

/**
 * @brief RawRows::seek locate at BEGIN or FLESH by inner
 * @param inner
 */
/* TODO
 * Optmz RawRows::seek(Ast *inner, int hint = -1/0/1)
 *   - if no token under r, c, search start from previous token
 *   - if hint == 0, do zigzag bi-dir search from curr r,c(locality)
 */
void Tokens::seek(const Ast *inner)
{
    for (auto rit = rows.begin(); rit != rows.end(); ++rit) {
        for (auto cit = rit->begin(); cit != rit->end(); ++cit) {
            if ((*cit)->getAst() == inner
                    && ((*cit)->getRole() == Token::Role::BEGIN
                        || (*cit)->getRole() == Token::Role::FLESH)) {
                seek(rit - rows.begin(), cit - rit->begin());
                qDebug() << "seeked at " << r << c;
                return;
            }
        }
    }
    assert(false && "seek: inner not found");
}

void Tokens::seek(const Ast *outer, size_t inner)
{
    assert(inner < outer->size());
    seek(&outer->at(inner));
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

void Tokens::light()
{
    assert(r < rows.size() && c < rows[r].size());

    const Token& under = *rows[r][c];
    assert(under.getRole() == Token::Role::BEGIN
           || under.getRole() == Token::Role::FLESH);

    size_t y = r, x = c;
    if (under.getRole() == Token::Role::BEGIN) {
        const Ast *ast = under.getAst();
        for (auto rit = rows.begin() + r; rit != rows.end(); ++rit) {
            // TODO (optmz): use cit = begin+c in first row
            for (auto cit = rit->begin(); cit != rit->end(); ++cit) {
                if ((*cit)->getAst() == ast
                        && (*cit)->getRole() == Token::Role::END) {
                    y = rit - rows.begin();
                    x = cit - rit->begin();
                    break;
                }
            }
        }
        assert(y != r || x != c);
    }

    Ast *outer = &under.getAst()->getParent();
    if (outer->getType() == Ast::Type::ROOT) {
        for (auto ob : observers)
            ob->observeLight(0, 0, 0, 0, r, c, y, x);
        return;
    }
    // TODO: optmz
    int lbr = -1, lbc, ler = -1, lec;
    for (auto rit = rows.begin(); rit != rows.end(); ++rit) {
        for (auto cit = rit->begin(); cit != rit->end(); ++cit) {
            const Ast *ast = (*cit)->getAst();
            if (ast == outer) {
                if ((*cit)->getRole() == Token::Role::BEGIN) {
                    lbr = rit - rows.begin();
                    lbc = cit - rit->begin();
                } else if ((*cit)->getRole() == Token::Role::END) {
                    ler = rit - rows.begin();
                    lec = cit - rit->begin();
                    break;
                }
            }
        }
    }

    assert(lbr != -1 && ler != -1);

    for (auto ob : observers)
        ob->observeLight(lbr, lbc, ler, lec, r, c, y, x);
}

void Tokens::newLine()
{
    bool chop = r < rows.size() && c < rows[r].size();
    seek(r + 1, 0);
    rows.emplace(rows.begin() + r);
    for (auto ob : observers)
        ob->observeNewRow();

    if (chop) {
        // TODO re-design newline() and insert.
        // (for the total system, including qml)
        // row insertion should be explicity reported
        // p.s. press enter is the only way to create new row.
    }
}

void Tokens::write(Token *token)
{
    std::vector<std::unique_ptr<Token>> &row = rows[r];
    row.emplace(row.begin() + c, token);
    for (auto ob : observers)
        ob->observeInsert(*token);

    seek(STAY, c + 1);
}

void Tokens::insert(const Ast &ast)
{
    hammer.write(ast);
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
    std::cout << "End of rawRows print" << std::endl;
}

void Tokens::registerObserver(TokensObserver *ob)
{
    observers.push_back(ob);
}


