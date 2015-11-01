#ifndef DOC_H
#define DOC_H

#include "core/rawrows.h"
#include "ast/ast.h"
#include "ast/rootast.h"
#include "mode/mode.h"
#include <memory>
#include <stack>

/* TODO RawRows::dark()
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

    void registerRawRowsObserver(RawRowsObserver *ob);

private:
    void fuckIn();
    void damnOut();
    void jackKick(int step);

private:
    std::unique_ptr<std::stack<std::shared_ptr<Mode>>> modeStack;
    RootAst root;
    RawRows rawRows;
    Ast *outer = &root;
    size_t inner = 0;
};

#endif // DOC_H
