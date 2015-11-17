#ifndef MENUMODE_H
#define MENUMODE_H

#include "sell/mode/mode.h"

class MenuMode : public Mode
{
public:
    MenuMode(Doc &doc, bool append);

    void keyboard(char key) override;

private:
    bool append;
};

#endif // MENUMODE_H
