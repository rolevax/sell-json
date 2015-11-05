#ifndef TOKENS_H
#define TOKENS_H

#include "hammer.h"
#include "token.h"
#include "tokensobserver.h"
#include <vector>

/* TokenSystem: {Bone tab}Flesh | {Bone tab}Begin...[newline]End
 *    - lightable region := Flesh | Begin...End
 *    - when removing a lightable region, clean all 'outside bone' tabs
 */
class Tokens
{
public:
    const static int STAY = -1;
    Tokens();

    Tokens(const Tokens&) = delete;
    Tokens &operator=(const Tokens&) = delete;

    void seek(ssize_t nextR, ssize_t nextC);
    void seek(const Ast *inner);
    void seek(const Ast *outer, size_t inner);
    void suck();
    void light();
    void newLine();
    void write(Token *token);
    void insert(const Ast &ast);
    void erase(size_t br, size_t bc, size_t er, size_t ec);
    void remove();
    void print();

    void registerObserver(TokensObserver *ob);

private:
    Hammer hammer;
    std::vector<std::vector<std::unique_ptr<Token>>> rows;
    std::vector<TokensObserver*> observers;
    size_t r = 0;
    size_t c = 0;
};

#endif // TOKENS_H
