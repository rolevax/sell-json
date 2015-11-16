#include "sell/mode/mode.h"

Mode::Mode(Type type, Doc &doc) :
    type(type),
    doc(doc)
{

}

Mode::Type Mode::getType() const
{
    return type;
}

