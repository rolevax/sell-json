#ifndef MENUMODE_H
#define MENUMODE_H

#include "sell/mode/mode.h"

class MenuMode : public Mode
{
public:
    enum class Context
    {
        INSERT, APPEND, ASSART, CHANGE, NEST
    };
    MenuMode(Doc &doc, Context context);

    void keyboard(char key) override;
    void emptyKeyboard(char key) override;
    void onPushed() override;
    void onPopped() override;
    const char *name() override;

private:
    void work(Ast::Type type, const char *keytal = nullptr);

private:
    Context context;
};

#endif // MENUMODE_H
