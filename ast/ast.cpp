#include "sell/ast/ast.h"

bool Ast::isList(Ast::Type type)
{
    return type == Type::ARRAY || type == Type::OBJECT;
}

bool Ast::isList(const Ast &a)
{
    return isList(a.type);
}

bool Ast::isScalar(Ast::Type type)
{
    return type == Type::KEY || type == Type::STRING
            || type == Type::NUMBER || type == Type::KEYTAL;
}

bool Ast::isScalar(const Ast &a)
{
    return isScalar(a.type);
}

Ast::Ast(Type t) :
    type(t)
{

}

Ast::Type Ast::getType() const
{
    return type;
}

Ast &Ast::getParent() const
{
    return *parent;
}

void Ast::setParent(Ast *p)
{
    parent = p;
    root = parent->root;
}

void Ast::insert(size_t pos, Ast *child)
{
    child->setParent(this);
    doInsert(pos, child);
}

std::unique_ptr<Ast> Ast::remove(size_t pos)
{
    (void) pos;
    throw 2048;
}

size_t Ast::size() const
{
    return 0;
}

Ast &Ast::at(size_t pos) const
{
    (void) pos;
    throw 2048;
}

size_t Ast::indexOf(const Ast *child) const
{
    (void) child;
    return -1;
}

void Ast::doInsert(size_t pos, Ast *child)
{
    (void) pos;
    (void) child;
    throw 2048;
}



