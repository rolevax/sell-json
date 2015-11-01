#ifndef SCALARAST_H
#define SCALARAST_H

#include "ast/ast.h"
#include <string>

class ScalarAst : public Ast
{
public:
    ScalarAst(Type t, const std::string &text);

    const std::string &getText() const;

    size_t size() const override;
    Ast &at(size_t pos) const override;

protected:
    void doInsert(size_t pos, std::unique_ptr<Ast> &child) override;

private:
    std::string text;
};

#endif // SCALARAST_H
