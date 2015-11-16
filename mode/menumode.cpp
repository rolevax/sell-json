#include "sell/mode/menumode.h"
#include "sell/mode/stringinputmode.h"
#include "sell/mode/numberinputmode.h"

MenuMode::MenuMode(Doc &doc) :
    Mode(doc)
{

}

void MenuMode::keyboard(char key)
{
    switch (key) {
    case ' ':
        leave();
        break;
    case 's':
        leave(new StringInputMode(doc));
        break;
    case 'n':
        leave(new NumberInputMode(doc));
        break;
    default:
        break;
    }
}


