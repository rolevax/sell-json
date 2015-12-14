#ifndef MENUMODE_H
#define MENUMODE_H

#include "sell/mode/mode.h"

class MenuMode : public Mode
{
public:
    enum class Context
    {
        INSERT, APPEND, ASSART, CHANGE
    };
    MenuMode(Doc &doc, Context context);

    void keyboard(char key) override;
    void onPushed() override;
    void onPopped() override;
    const char *name() override;

private:
    void prepareCursor();

private:
    Context context;
};

#endif // MENUMODE_H
