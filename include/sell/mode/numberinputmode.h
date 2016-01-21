#ifndef NUMBERINPUTMODE_H
#define NUMBERINPUTMODE_H

#include "sell/mode/mode.h"

class NumberInputMode : public Mode
{
public:
    NumberInputMode(EditableDoc &doc, bool clear);

    void keyboard(char key) override;
    void onPushed() override;
    void onPopped() override;
    const char *name() override;

private:
    bool clear;
};

#endif // NUMBERINPUTMODE_H
