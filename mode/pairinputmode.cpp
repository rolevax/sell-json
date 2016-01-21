#include "sell/core/editabledoc.h"
#include "sell/mode/pairinputmode.h"
#include "sell/mode/menumode.h"
#include "sell/mode/stringinputmode.h"

#include <iostream>

PairInputMode::PairInputMode(EditableDoc &doc)
    : Mode(doc)
{

}

void PairInputMode::onPushed()
{
    doc.fallIn();
    stage = Stage::KEY_DONE;
    doc.push(new StringInputMode(doc, true));
}

void PairInputMode::onResume()
{
    switch (stage) {
    case Stage::KEY_DONE:
        doc.sibling(+1);
        stage = Stage::VALUE_DONE;
        doc.push(new MenuMode(doc, MenuMode::Context::CHANGE));
        break;
    case Stage::VALUE_DONE:
        doc.digOut();
        doc.pop();
        break;
    }
}

const char *PairInputMode::name()
{
    return "Pair Input";
}
