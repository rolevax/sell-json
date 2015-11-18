#ifndef DOC_H
#define DOC_H

#include "sell/core/tokens.h"
#include "sell/ast/ast.h"
#include "sell/ast/rootast.h"
#include "sell/mode/mode.h"
#include <memory>
#include <stack>

class PDoc;

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
    void registerObserver(PDoc *ob);

    friend class Mode;

private:
    void push(Mode *mode);
    void pop();

    void fuckIn();
    void damnOut();
    void jackKick(int step);
    void insert(Ast::Type type);
    void remove();
    void showMenu(const char *text);

private:
    std::stack<std::unique_ptr<Mode>> modes;
    RootAst root;
    Tokens tokens;
    Ast *outer = &root;
    size_t inner = 0;
    PDoc *ob = nullptr;
};

#endif // DOC_H
