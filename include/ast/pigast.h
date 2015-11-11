#ifndef PIGAST_H
#define PIGAST_H

#include "core/bonetoken.h"
#include "ast/ast.h"
#include <vector>
#include <memory>

class PigAst : public Ast
{
public:
    PigAst(Type t);

    std::unique_ptr<Ast> remove(size_t pos) override;
    size_t indexOf(const Ast *child) const override;

protected:
    std::vector<std::unique_ptr<Ast>> subtrees;
};

#endif // PIGAST_H
