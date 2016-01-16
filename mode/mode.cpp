#include "sell/mode/mode.h"
#include "sell/core/doc.h"

Mode::Mode(Doc &doc) :
    doc(doc),
    outer(doc.outer),
    inner(doc.inner),
    tokens(doc.tokens)
{

}

void Mode::push(Mode *mode)
{
    doc.push(mode);
}

void Mode::leave(Mode *next)
{
    Doc *pd = &doc; // for memory safety
    doc.pop(); // cause object suicide
    /* Do not use "this" pointer below */
    if (next != nullptr)
        pd->push(next);
}

void Mode::fuckIn()
{
    doc.fuckIn();
}

void Mode::damnOut()
{
    doc.damnOut();
}

void Mode::jackKick(int step)
{
    doc.jackKick(step);
}

void Mode::insert(Ast::Type type)
{
    doc.insert(type);
}

void Mode::remove()
{
    doc.remove();
}

void Mode::change(Ast::Type type)
{
    doc.change(type);
}

void Mode::toggleTension(bool b)
{
    doc.toggleTension(b);
}

