#include "sell/mode/pairinputmode.h"
#include "sell/mode/menumode.h"
#include "sell/mode/stringinputmode.h"

#include <iostream>

PairInputMode::PairInputMode(Doc &doc)
    : Mode(doc)
{

}

void PairInputMode::onPushed()
{
    fallIn();
    stage = Stage::KEY_DONE;
    push(new StringInputMode(doc, true));
}

void PairInputMode::onResume()
{
    switch (stage) {
    case Stage::KEY_DONE:
        sibling(+1);
        stage = Stage::VALUE_DONE;
        push(new MenuMode(doc, MenuMode::Context::CHANGE));
        break;
    case Stage::VALUE_DONE:
        digOut();
        pop();
        break;
    }
}

const char *PairInputMode::name()
{
    return "Pair Input";
}
