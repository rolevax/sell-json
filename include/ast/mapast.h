#ifndef MAPAST_H
#define MAPAST_H

#include "ast/pigast.h"

class MapAst : public PigAst
{
public:
    MapAst(Type t);

    size_t size() const override;
    Ast &at(size_t pos) const override;

protected:
    void doInsert(size_t pos, std::unique_ptr<Ast> &child) override;

private:
    std::vector<Type> keys;
};

#endif // MAPAST_H
