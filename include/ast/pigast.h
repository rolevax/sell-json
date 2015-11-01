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

protected:
    std::vector<std::unique_ptr<Ast>> subtrees;
};

#endif // PIGAST_H
