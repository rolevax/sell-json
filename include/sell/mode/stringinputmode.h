#ifndef STRINGINPUTMODE_H
#define STRINGINPUTMODE_H

#include "sell/mode/mode.h"

class StringInputMode : public Mode
{
public:
    StringInputMode(Doc &doc);

    void keyboard(char key) override;
    void onPushed() override;
    void onPopped() override;
    const char *name() override;
};

#endif // STRINGINPUTMODE_H
