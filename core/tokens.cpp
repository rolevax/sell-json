#include "core/tokens.h"
#include <QDebug>
#include <iostream>

#include <cassert>

Tokens::Tokens()
{
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
    if (inner < outer->size())  {
        seek(&outer->at(inner));
    } else if (inner == outer->size()){
        // TODO: Ast::isList(Type)
        assert(outer->getType() == Ast::Type::ARRAY
               || outer->getType() == Ast::Type::OBJECT);
        // TODO XXX
        /*
         * when press 'i' to insert, use this seek() to locate the
         * first token behind the previous token which does not
         * belong to the previous token (next to end/flesh)
         * skip separator token of parent AST if any.
         *
         * don't consider map insert for now. that's not in JSON anyway.
         * simply forbid point to end in a map ast (Pair of JSON)
         * don't consider root insert.
         * root's (only) child can be only changed or deleted.
         *
         * plan: impl Hammer, regression test. then insert.
         * Tokens::Tokens(Hammer)
         * vector<vector<Token>> Hammer::hit(Ast)
         *   - return a well-indented vector<vector<>>
         *   - indent level calculated by distance to root
         *   - main logic about recursive indent management
         * Tokens::insert(Ast) { insert(hammer(ast)); } very simple.
         */
        ///seek(&outer->at(inner - 1));
    } else {
        throw -123;
    }
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
    seek(r + 1, 0);
    rows.emplace(rows.begin() + r);
}

void Tokens::insert(Token *token)
{
    if (r == rows.size()) // new row
        rows.emplace_back();

    std::vector<std::unique_ptr<Token>> &row = rows[r];
    row.emplace(row.begin() + c, token);
    for (auto ob : observers)
        ob->observeInsert(*token);

    seek(STAY, c + 1);
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

void Tokens::registerObserver(RawRowsObserver *ob)
{
    observers.push_back(ob);
}


