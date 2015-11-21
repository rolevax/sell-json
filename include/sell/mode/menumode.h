#ifndef MENUMODE_H
#define MENUMODE_H

#include "sell/mode/mode.h"

class MenuMode : public Mode
{
public:
    MenuMode(Doc &doc, bool append, bool empty);

    void keyboard(char key) override;
    void onPushed() override;
    void onPopped() override;
    const char *name() override;

private:
    void prepareCursor();

private:
    bool append;
    bool empty;
};

#endif // MENUMODE_H
