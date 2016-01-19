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

void Mode::pop(Mode *next)
{
    doc.pop(next);
}

void Mode::fuckIn()
{
    doc.fuckIn();
}

void Mode::damnOut()
{
    doc.damnOut();
}

void Mode::sibling(int step)
{
    doc.sibling(step);
}

void Mode::jackKick(bool down)
{
    doc.jackKick(down);
}

void Mode::insert(Ast::Type type)
{
    doc.insert(type);
}

std::unique_ptr<Ast> Mode::remove()
{
    return doc.remove();
}

void Mode::change(Ast::Type type)
{
    doc.change(type);
}

void Mode::nest(Ast::Type type)
{
    doc.nest(type);
}

void Mode::toggleTension(bool b)
{
    doc.toggleTension(b);
}

