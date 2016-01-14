#ifndef AST_H
#define AST_H

#include <cstddef>
#include <memory>

class RootAst;

class Ast
{
public:
    enum class Type {
        ROOT, ARRAY, OBJECT, PAIR, KEY, STRING, NUMBER, KEYTAL
    };

    static bool isList(Type type);
    static bool isList(const Ast &a);
    static bool isScalar(Type type);
    static bool isScalar(const Ast &a);

    Ast(Type t);

    Type getType() const;
    Ast &getParent() const;
    void setParent(Ast *p);

    void insert(size_t pos, Ast *child);
    virtual std::unique_ptr<Ast> remove(size_t pos);
    virtual size_t size() const;
    virtual Ast &at(size_t pos) const;
    virtual size_t indexOf(const Ast *child) const;

protected:
    virtual void doInsert(size_t pos, Ast *child);

protected:
    Type type;
    Ast *parent = nullptr;
    RootAst *root = nullptr;
};

#endif // AST_H
