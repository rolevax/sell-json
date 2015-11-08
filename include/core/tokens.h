#ifndef TOKENS_H
#define TOKENS_H

#include "hammer.h"
#include "token.h"
#include "tokensobserver.h"
#include <vector>

/* TokenGroup     := [Bone tab] LightableGroup
 * LightableGroup := Begin Flesh [newline] End
 *                 | Begin TokenGroup {TokenGroup} [newline] End
 */
class Tokens
{
public:
    const static int STAY = -1;
    Tokens();

    Tokens(const Tokens&) = delete;
    Tokens &operator=(const Tokens&) = delete;

    Region locate(const Ast *tar);
    void suck(Region &region);
    void light(const Ast *inner);
    void newLine(size_t r, size_t c);
    void write(Token *token, size_t r, size_t c);
    void insert(const Ast *outer, size_t inner);
    void erase(const Region &r);
    void remove(const Ast *outer, size_t inner);
    void print();

    void registerObserver(TokensObserver *ob);

private:
    Hammer hammer;
    std::vector<std::vector<std::unique_ptr<Token>>> rows;
    std::vector<TokensObserver*> observers;
};

#endif // TOKENS_H
