#ifndef MENUMODE_H
#define MENUMODE_H

#include "sell/mode/mode.h"

class MenuMode : public Mode
{
public:
    MenuMode(Doc &doc);

    void keyboard(char key) override;
};

#endif // MENUMODE_H
