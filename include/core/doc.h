#ifndef DOC_H
#define DOC_H

#include "core/tokens.h"
#include "ast/ast.h"
#include "ast/rootast.h"
#include <memory>
#include <stack>

/* TODO Tokens::dark()
 *   - trigger when root has no child (new doc/all deleted)
 */
class Doc
{
public:
    Doc();

    Doc(const Doc&) = delete;
    Doc &operator=(const Doc&) = delete;

    void load();
    void keyboard(char key);

    void registerRawRowsObserver(TokensObserver *ob);

private:
    void fuckIn();
    void damnOut();
    void jackKick(int step);
    void insert();
    void remove();

private:
    RootAst root;
    Tokens tokens;
    Ast *outer = &root;
    size_t inner = 0;
};

#endif // DOC_H
