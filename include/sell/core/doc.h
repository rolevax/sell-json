#ifndef DOC_H
#define DOC_H

#include "sell/core/tokens.h"
#include "sell/ast/ast.h"
#include "sell/ast/rootast.h"
#include "sell/mode/mode.h"
#include <memory>
#include <stack>

class PDoc;

class Doc
{
public:
    Doc();

    Doc(const Doc&) = delete;
    Doc &operator=(const Doc&) = delete;

    void load(const std::string &filename);
    void save(const std::string &filename);
    void keyboard(char key);

    void registerTokensObserver(TokensObserver *ob);
    void registerObserver(PDoc *ob);

    friend class Mode;

private:
    void push(Mode *mode);
    void pop(Mode *nextPush);

    void fallIn();
    void digOut();
    void sibling(int step);
    void jackKick(bool down);
    void insert(Ast::Type type);
    std::unique_ptr<Ast> remove();
    void change(Ast::Type type);
    void nest(Ast::Type type);
    void toggleTension(bool b);

    Ast *newTree(Ast::Type type);

private:
    std::stack<std::unique_ptr<Mode>> modes;
    RootAst root;
    Tokens tokens;
    Ast *outer = &root;
    size_t inner = 0;
    PDoc *ob = nullptr;
};

#endif // DOC_H
