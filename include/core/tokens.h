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
    struct Region
    {
        int br;
        int bc;
        int er;
        int ec;
    };

    const static int STAY = -1;
    Tokens();

    Tokens(const Tokens&) = delete;
    Tokens &operator=(const Tokens&) = delete;

    Region locate(const Ast *tar);
    void suck();
    void light(const Ast *inner);
    void newLine(size_t r, size_t c);
    void write(Token *token, size_t r, size_t c);
    void insert(const Ast *outer, size_t inner);
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
