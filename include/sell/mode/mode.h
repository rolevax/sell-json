#ifndef MODE_H
#define MODE_H

#include "sell/ast/ast.h"

class Doc;
class Tokens;

class Mode
{
public:
    Mode(const Mode& copy) = delete;
    Mode &operator=(const Mode& assign) = delete;

    virtual void keyboard(char key) = 0;
    virtual void onPushed() {}
    virtual void onPopped() {}
    virtual const char *name() = 0;

protected:
    void push(Mode *mode);
    void leave(Mode *next = nullptr);

    void fuckIn();
    void damnOut();
    void jackKick(int step);
    void insert(Ast::Type type);
    void remove();
    void change(Ast::Type type);
    void showMenu(const char *text);

protected:
    Mode(Doc &doc);
    Doc &doc;
    Ast *&outer;
    size_t &inner;
    Tokens &tokens;
};

#endif // MODE_H
