#include "sell/core/tokens.h"
#include "sell/core/soultoken.h"
#include <cassert>

#include <QDebug>
#include <iostream>
#include <sstream>

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
 * Also called in Doc::change().
 * When changing a node inside map, will locate by the
 * meta soul token left by Tokens::remove().
 */
void Tokens::insert(const Ast *outer, size_t inner)
{
    assert(outer->size() > 0 && inner < outer->size());

    if (outer->getType() == Ast::Type::ROOT) { // outer is root
        hammer.hit(*outer, 0, 0);
    } else if (Ast::isList(*outer)) {
        if (outer->size() == 1) { // assart
            Ast *parent = &outer->getParent();
            size_t outerIndex = parent->indexOf(outer);
            remove(parent, outerIndex);
            insert(parent, outerIndex);
            /* recursion terminates when outer is root */
        } else if (inner < outer->size() - 1) { // not very end
            Region younger = locate(&outer->at(inner + 1));
            suck(younger);
            hammer.hit(outer->at(inner), younger.br, younger.bc);
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
    } else if (Ast::isMap(*outer)) {
        Region out = locate(outer);
        // works only for JSON
        std::vector<std::unique_ptr<Token>> &row = rows[out.br];
        auto it = std::find_if(row.begin() + out.bc, row.begin() + out.ec + 1,
                               [](const std::unique_ptr<Token> &t) {
                                   return t->getRole() == Token::Role::META;
                               });
        assert(it != row.end());
        size_t bc = it - row.begin();
        row.erase(it); // delete meta token
        hammer.hit(outer->at(inner), out.br, bc);
    } else {
        throw "Tokens::insert uninsertable outer";
    }
}

/**
 * @brief Tokens::remove
 * @param outer Parent of the node to remove
 * @param inner Index of the node to remove within its parent
 * This should be called before modifying Ast.
 * If outer is map, will leave a meta placeholder token.
 */
void Tokens::remove(const Ast *outer, size_t inner)
{
    assert(inner < outer->size());

    Region r = locate(&outer->at(inner));
    suck(r);
    erase(r);

    if (Ast::isMap(*outer)) {
        Token *meta = new SoulToken(outer, Token::Role::META);
        rows[r.br].emplace(rows[r.br].begin() + r.bc, meta);
    }
}

void Tokens::updateScalar(const Ast *outer, size_t inner)
{
    assert(Ast::isScalar(outer->at(inner)));
    Region r = locate(&outer->at(inner));

    for (auto ob : observers)
        ob->observeUpdateLine(r.br, pluck(r.br));
}

std::string Tokens::pluck(size_t r)
{
    assert(r < rows.size());

    std::stringstream ss;
    for (const std::unique_ptr<Token> &t : rows[r])
        ss << t->getText();

    return ss.str();
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

/**
 * @brief Convert token level index into character level index
 * @param r row of token
 * @param c column of token
 * @return column index in terms of character
 */
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
 * @brief Find a "LightableGroup" by Ast
 * @param tar The node to locate
 * @return A Region representing a "LightableGroup"
 * See the comment in Tokens class defination for "LightableGroup".
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
 * @brief Change LightableGroup into TokenGroup
 * @param r
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

    if (r.br == r.er) { // one-line erase
        needJoin += rows[r.er][r.ec]->needNewLine();
        auto it = rows[r.br].begin();
        rows[r.br].erase(it + r.bc, it + r.ec + 1);
    } else { // multi-line erase
        needJoin += rows[r.br].back()->needNewLine(); // first line

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
        joinLine(r.br + 1);
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

/**
 * @brief Join the r-th line into the (r-1)-th line
 * @param r
 */
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


