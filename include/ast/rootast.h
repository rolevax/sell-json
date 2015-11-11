#ifndef ROOTAST_H
#define ROOTAST_H

#include "ast/ast.h"

class RootAst : public Ast
{
public:
    RootAst();

    bool isPresent() const;

    size_t size() const override;
    Ast &at(size_t pos) const override;
    std::unique_ptr<Ast> remove(size_t pos) override;
    size_t indexOf(const Ast *child) const override;

protected:
    void doInsert(size_t pos, std::unique_ptr<Ast> &child) override;

private:
    std::unique_ptr<Ast> subtree = nullptr;
    bool present = false;
};

#endif // ROOTAST_H
