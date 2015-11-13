#ifndef DOC_H
#define DOC_H

#include "core/tokens.h"
#include "ast/ast.h"
#include "ast/rootast.h"
#include <memory>
#include <stack>

enum class Mode
{
    VIEW, TYPE_MENU, INPUT_SCALAR,
};

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
    void push(Mode mode);
    void pop();
    void keyView(char key);
    void keyInput(char key);
    void keyMenu(char key);

    void fuckIn();
    void damnOut();
    void jackKick(int step);
    void insert(Ast::Type type);
    void remove();

private:
    std::stack<Mode> modes;
    RootAst root;
    Tokens tokens;
    Ast *outer = &root;
    size_t inner = 0;
};

#endif // DOC_H
