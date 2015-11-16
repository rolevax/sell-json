#ifndef DOC_H
#define DOC_H

#include "sell/core/tokens.h"
#include "sell/ast/ast.h"
#include "sell/ast/rootast.h"
#include "sell/mode/mode.h"
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
    void push(Mode::Type modeType);
    void pop();
    void keyView(char key);
    void keyInputString(char key);
    void keyInputNumber(char key);
    void keyMenu(char key);

    void fuckIn();
    void damnOut();
    void jackKick(int step);
    void insert(Ast::Type type);
    void remove();

private:
    std::stack<std::unique_ptr<Mode>> modes;
    RootAst root;
    Tokens tokens;
    Ast *outer = &root;
    size_t inner = 0;
};

#endif // DOC_H
