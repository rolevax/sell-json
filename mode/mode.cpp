#include "sell/mode/mode.h"

Mode::Mode(Type type) :
    type(type)
{

}

Mode::Type Mode::getType() const
{
    return type;
}

