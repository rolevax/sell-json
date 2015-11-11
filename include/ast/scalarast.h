#ifndef SCALARAST_H
#define SCALARAST_H

#include "ast/ast.h"
#include <string>

class ScalarAst : public Ast
{
public:
    ScalarAst(Type t, const std::string &text);

    const std::string &getText() const;

    void append(char c);

private:
    std::string text;
};

#endif // SCALARAST_H
