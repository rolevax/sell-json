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

    void setHotLight(bool b);
    void light(const Ast *inner);
    void insert(const Ast *outer, size_t inner);
    void remove(const Ast *outer, size_t inner);
    void updateScalar(const Ast *outer, size_t inner);
    void print();

    void registerObserver(TokensObserver *ob);

    friend class Hammer;

private:
    Region locate(const Ast *tar);
    void suck(Region &region);
    void write(Token *token, size_t r, size_t c);
    void erase(const Region &r);
    void updateFlesh(const Region &r);
    void newLine(size_t r, size_t c);
    void mergeLine(size_t r);

private:
    Hammer hammer;
    std::vector<std::vector<std::unique_ptr<Token>>> rows;
    std::vector<TokensObserver*> observers;
};

#endif // TOKENS_H
