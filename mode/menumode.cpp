#include "sell/mode/menumode.h"
#include "sell/mode/stringinputmode.h"
#include "sell/mode/numberinputmode.h"

MenuMode::MenuMode(Doc &doc, bool append) :
    Mode(doc),
    append(append)
{

}

void MenuMode::keyboard(char key)
{
    switch (key) {
    case ' ':
        leave();
        break;
    case 's':
        if (append)
            ++inner;
        leave(new StringInputMode(doc));
        break;
    case 'n':
        if (append)
            ++inner;
        leave(new NumberInputMode(doc));
        break;
    default:
        break;
    }
}


