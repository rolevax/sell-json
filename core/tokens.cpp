#include "sell/core/tokens.h"
#include <cassert>

#include <QDebug>
#include <iostream>

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
    Region in = anchor(locate(inner));
    Ast *outer = &inner->getParent();
    if (outer->getType() == Ast::Type::ROOT) {
        for (auto ob : observers)
            ob->observeLight(0, 0, 0, 0,
                             in.br, in.bc, in.er, in.ec);
    } else {
        Region out = anchor(locate(outer));
        for (auto ob : observers)
            ob->observeLight(out.br, out.bc, out.er, out.ec,
                             in.br, in.bc, in.er, in.ec);
    }
}

/**
 * @brief Tokens::insert
 * @param outer Parent of new node
 * @param inner Index of new node within its parent
 * Apply a insertion change in Ast into Tokens.
 * Should be called after Ast's insertion.
 */
void Tokens::insert(const Ast *outer, size_t inner)
{
    assert(outer->size() > 0 && inner < outer->size());

    if (outer->getType() == Ast::Type::ROOT) { // outer is root
        hammer.hit(*outer, 0, 0);
    } else if (outer->size() == 1) { // assart
        Ast *parent = &outer->getParent();
        size_t outerIndex = parent->indexOf(outer);
        remove(parent, outerIndex);
        insert(parent, outerIndex);
        /* recursion terminates when outer is root */
    } else if (inner < outer->size() - 1) { // not very end
        Region prevHead = locate(&outer->at(inner + 1));
        suck(prevHead);
        hammer.hit(outer->at(inner), prevHead.br, prevHead.bc);
    } else { // very end
        Region ass = locate(&outer->at(inner - 1));
        size_t nextR, nextC;
        if (rows[ass.er][ass.ec]->needNewLine()) {
            nextR = ass.er + 1;
            nextC = 0;
        } else {
            nextR = ass.er;
            nextC = ass.ec + 1;
        }
        hammer.hit(outer->at(inner), nextR, nextC);
    }
}

/**
 * @brief Tokens::remove
 * @param outer Parent of the node to remove
 * @param inner Index of the node to remove within its parent
 * This should be called before Ast's remove operation.
 */
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

size_t Tokens::anchor(size_t r, size_t c)
{
    assert(r < rows.size() && c <= rows[r].size());
    size_t offset = 0;
    for (size_t i = 0; i < c; i++)
        offset += rows[r][i]->getText().size();
    return offset;
}

Region Tokens::anchor(const Region &r)
{
   Region c(r);
   c.bc = anchor(c.br, c.bc);
   c.ec = anchor(c.er, c.ec);
   return c;
}

void Tokens::registerObserver(TokensObserver *ob)
{
    observers.push_back(ob);
}

/**
 * @brief Tokens::locate
 * @param tar The node to locate
 * @return A Region representing a "TokenGroup"
 * See the comment in Tokens class defination for "TokenGroup".
 */
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
#ifdef NDEBUG
    (void) found;
#endif
    return res;
}

/**
 * @brief Tokens::suck
 * @param r
 * Change "TokenGroup" into "lightableGroup"
 * See comment in Tokens class defination for details
 */
void Tokens::suck(Region &r)
{
    assert(r.br < rows.size() && r.bc < rows[r.br].size());
    Token::Role role = rows[r.br][r.bc]->getRole();
    if (role == Token::Role::BEGIN) {
        const Ast *ast = rows[r.br][r.bc]->getAst();
        if (r.bc > 0 && rows[r.br][r.bc - 1]->getAst() == ast)
            --r.bc; // skip one space indentation token
    }
}

/**
 * @brief Tokens::write
 * @param token
 * @param r row index
 * @param c column index
 * @return whether the write triggered a new line
 * Insert a token at specified position.
 * Automatically handles new line according to token->needNewLine().
 * Also add a new line if the previous token need one.
 */
bool Tokens::write(Token *token, size_t r, size_t c)
{
    assert(r < rows.size() && c <= rows[r].size());

    bool ret = false;

    // TODO: really needed? may kind of assert 'never write after newline'
    if (c > 0 && rows[r][c - 1]->needNewLine()) {
        newLine(r, c);
        ++r;
        c = 0;
        ret = true;
    }

    std::vector<std::unique_ptr<Token>> &row = rows[r];
    row.emplace(row.begin() + c, token);
    for (auto ob : observers)
        ob->observeWrite(*token, r, anchor(r, c));

    // one write triggers at most one new line
    if (!ret && token->needNewLine()) {
        newLine(r, c + 1);
        ret = true;
    }

    return ret;
}

/**
 * @brief Tokens::erase
 * @param r
 * Remove all tokens from the region 'r'
 * If 'r' indicates multiple lines,
 * all lines except the first and the last will be removed
 * without calling joinLine() function.
 * The observer should be aware of this.
 */
void Tokens::erase(const Region &r)
{
    assert(r.br < rows.size() && r.bc < rows[r.br].size());
    assert(r.er < rows.size() && r.ec < rows[r.er].size());
    assert(r.br <= r.er);

    Region cr = anchor(r);
    int needJoin = 0;
    needJoin += rows[r.br].back()->needNewLine(); // first line

    if (r.br == r.er) { // one-line erase
        auto it = rows[r.br].begin();
        rows[r.br].erase(it + r.bc, it + r.ec + 1);
    } else { // multi-line erase
        /* bottom-up remove from the last row
         * since it won't mess up the row indices
         */

        // remove inside the last line
        needJoin += rows[r.er][r.ec]->needNewLine();
        auto endIt = rows[r.er].begin();
        rows[r.er].erase(endIt, endIt + r.ec + 1);

        if (r.br + 1 < r.er) // remove internal lines
            rows.erase(rows.begin() + r.br + 1, rows.begin() + r.er);

        // remove inside the first line
        rows[r.br].erase(rows[r.br].begin() + r.bc, rows[r.br].end());
    }

    for (auto ob : observers)
        ob->observeErase(cr);

    while (needJoin --> 0)
        joinLine(r.br);
}

void Tokens::updateFlesh(const Region &r)
{
    Token &t = *rows[r.br][r.bc + 1];
    assert(t.getRole() == Token::Role::FLESH);
    Region cr = anchor(r);
    for (auto ob : observers)
        ob->observeUpdateFlesh(cr.br, cr.bc, cr.ec, t);
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
        ob->observeNewLine(r, anchor(r, c));
}

void Tokens::joinLine(size_t r)
{
    assert(r > 0 && r < rows.size());
    auto &prevRow = rows[r - 1];
    prevRow.insert(prevRow.end(),
                   std::make_move_iterator(rows[r].begin()),
                   std::make_move_iterator(rows[r].end()));
    rows.erase(rows.begin() + r);

    for (auto ob : observers)
        ob->observeJoinLine(r);
}


