#ifndef DOC_H
#define DOC_H

#include "sell/core/editabledoc.h"
#include "sell/core/tokens.h"
#include "sell/ast/rootast.h"
#include <stack>

class PDoc;

class Doc : public EditableDoc
{
public:
    Doc(PDoc *ob);

    Doc(const Doc&) = delete;
    Doc &operator=(const Doc&) = delete;

    void load(const std::string &filename);
    void save(const std::string &filename);
    void keyboard(char key);

    void registerTokensObserver(TokensObserver *ob);

private:
    /// @name EditableDoc interface
    ///@{
    void push(Mode *mode) override;
    void pop(Mode *nextPush) override;

    Ast::Type outerType() override;
    Ast::Type innerType() override;

    void cursorIn() override;
    void cursorForward() override;

    void fallIn() override;
    void digOut() override;
    void sibling(int step) override;
    void jackKick(bool down) override;
    void hackLead(bool right) override;
    void insert(Ast::Type type) override;
    std::unique_ptr<Ast> remove() override;
    void change(Ast::Type type) override;
    void nest(Ast::Type type) override;

    void scalarAppend(const char *str) override;
    void scalarAppend(char c) override;
    void scalarClear() override;

    void setHotLight(bool b) override;
    void toggleTension(bool b) override;
    ///@}

    /// @name "really" private functions
    ///@{
    Ast *newTree(Ast::Type type);
    ScalarAst &getScalar();
    ///@}

private:
    std::stack<std::unique_ptr<Mode>> modes;
    RootAst root;
    Tokens tokens;
    Ast *outer = &root;
    size_t inner = 0;
    PDoc *ob;
};

#endif // DOC_H
