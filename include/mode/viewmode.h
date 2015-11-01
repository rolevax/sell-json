#ifndef VIEWMODE_H
#define VIEWMODE_H

#include "mode.h"

class ViewMode : public Mode
{
public:
    ViewMode();
    void execute(int key) override;
};

#endif // VIEWMODE_H
