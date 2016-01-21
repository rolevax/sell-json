#ifndef SCALARAST_H
#define SCALARAST_H

#include "sell/ast/ast.h"
#include <string>

class ScalarAst : public Ast
{
public:
    ScalarAst(Type t, const std::string &text);

    ScalarAst *clone() const override;

    const std::string &getText() const;

    void append(char c);
    void clear();

private:
    std::string text;
};

#endif // SCALARAST_H
