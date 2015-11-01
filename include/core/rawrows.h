#ifndef RAWROWS_H
#define RAWROWS_H

#include "token.h"
#include "rawrowsobserver.h"
#include <vector>

/* TokenSystem: {Bone tab}Flesh | {Bone tab}Begin...[newline]End
 *    - lightable region := Flesh | Begin...End
 *    - when removing a lightable region, clean all 'outside bone' tabs
 */
class RawRows
{
public:
    const static int STAY = -1;
    RawRows();

    RawRows(const RawRows&) = delete;
    RawRows &operator=(const RawRows&) = delete;

    void seek(ssize_t nextR, ssize_t nextC);
    void seek(const Ast *inner);
    void light();
    void newLine();
    void insert(Token *token);
    void print();

    void registerObserver(RawRowsObserver *ob);

private:
    std::vector<std::vector<std::unique_ptr<Token>>> rows;
    std::vector<RawRowsObserver*> observers;
    size_t r = 0;
    size_t c = 0;
};

#endif // RAWROWS_H
