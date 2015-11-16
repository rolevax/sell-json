#ifndef NUMBERINPUTMODE_H
#define NUMBERINPUTMODE_H

#include "sell/mode/mode.h"

class NumberInputMode : public Mode
{
public:
    NumberInputMode(Doc &doc);

    void keyboard(char key) override;
    void onPushed() override;
    void onPopped() override;
};

#endif // NUMBERINPUTMODE_H
