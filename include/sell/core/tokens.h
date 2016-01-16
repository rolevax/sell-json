#ifndef TOKENS_H
#define TOKENS_H

#include "hammer.h"
#include "token.h"
#include "tokensobserver.h"
#include <vector>

/* TokenGroup     := [Bone tab] LightableGroup
 * LightableGroup := Begin Flesh End [newline]
 *                 | Begin TokenGroup {TokenGroup} End [newline]
 */
/* TODO: re-arrange functions in cpp file
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
    std::string pluck(size_t r);
    void print();

    void registerObserver(TokensObserver *ob);

    /** @name Hammer's Interface
     */
    ///@{
    bool write(Token *token, size_t r, size_t c);
    void erase(const Region &r);
    ///@}

private:
    Region locate(const Ast *tar);
    void suck(Region &region);
    void newLine(size_t r, size_t c);
    void joinLine(size_t r);
    size_t anchor(size_t r, size_t c);
    Region anchor(const Region &r);

private:
    Hammer hammer;
    std::vector<std::vector<std::unique_ptr<Token>>> rows;
    std::vector<TokensObserver*> observers;
};

#endif // TOKENS_H
