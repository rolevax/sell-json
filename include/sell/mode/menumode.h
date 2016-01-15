#ifndef MENUMODE_H
#define MENUMODE_H

#include "sell/mode/mode.h"

class MenuMode : public Mode
{
public:
    enum class Context
    {
        /*
         * Many 'if' conditions assumes that there can be
         * only these four values. If this enum is going
         * to be re-defined, you should search around and
         * change a bunch of 'if' conditions.
         */
        INSERT, APPEND, ASSART, CHANGE
    };
    MenuMode(Doc &doc, Context context);

    void keyboard(char key) override;
    void onPushed() override;
    void onPopped() override;
    const char *name() override;

private:
    void work(Ast::Type type, const char *keytal = nullptr);

private:
    Context context;
};

#endif // MENUMODE_H
