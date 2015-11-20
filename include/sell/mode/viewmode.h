#ifndef VIEWMODE_H
#define VIEWMODE_H

#include "sell/mode/mode.h"

class ViewMode : public Mode
{
public:
    ViewMode(Doc &doc);

    void keyboard(char key) override;
    const char *name() override;
};

#endif // VIEWMODE_H
