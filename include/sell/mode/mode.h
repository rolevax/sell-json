#ifndef MODE_H
#define MODE_H

#include "sell/ast/ast.h"

class EditableDoc;

class Mode
{
public:
    Mode(const Mode& copy) = delete;
    Mode &operator=(const Mode& assign) = delete;
    virtual ~Mode() = default;

    virtual void keyboard(char key) { (void) key; }
    virtual void emptyKeyboard(char key) { (void) key; }
    virtual void onPushed() {}
    virtual void onPopped() {}
    virtual void onResume() {}
    virtual const char *name() = 0;

protected:
    Mode(EditableDoc &doc) : doc(doc) {}
    EditableDoc &doc;
};

#endif // MODE_H
