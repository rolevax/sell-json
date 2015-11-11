#include "ast/ast.h"

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

void Ast::insert(size_t pos, std::unique_ptr<Ast> &child)
{
    child->setParent(this);
    doInsert(pos, child);
}

size_t Ast::indexOf(const Ast *child) const
{
    (void) child;
    return -1;
}


