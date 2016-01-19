#ifndef PAIRINPUTMODE_H
#define PAIRINPUTMODE_H

#include "sell/mode/mode.h"

class PairInputMode : public Mode
{
public:
    PairInputMode(Doc &doc);

    void onPushed() override;
    void onResume() override;
    const char *name() override;

private:
    enum class Stage { KEY_DONE, VALUE_DONE };
    Stage stage;
};

#endif // PAIRINPUTMODE_H
