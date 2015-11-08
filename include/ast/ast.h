#ifndef AST_H
#define AST_H

#include <cstddef>
#include <memory>

class RootAst;

class Ast
{
public:
    enum class Type {
        ROOT, ARRAY, OBJECT, PAIR, KEY, SCALAR
    };

    Ast(Type t);

    Type getType() const;
    Ast &getParent() const;
    void setParent(Ast *p);

    void insert(size_t pos, std::unique_ptr<Ast> &child);
    virtual std::unique_ptr<Ast> remove(size_t pos) = 0;
    virtual size_t size() const = 0;
    virtual Ast &at(size_t pos) const = 0;

protected:
    virtual void doInsert(size_t pos, std::unique_ptr<Ast> &child) = 0;

protected:
    Type type;
    Ast *parent = nullptr;
    RootAst *root = nullptr;
};

#endif // AST_H
