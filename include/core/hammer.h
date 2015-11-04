#ifndef RAWROWSWRITER_H
#define RAWROWSWRITER_H

#include "core/rawrows.h"
#include "ast/scalarast.h"
#include "ast/listast.h"
#include "ast/mapast.h"

class RawRowsWriter
{
public:
    RawRowsWriter(RawRows &rawRows);

    RawRowsWriter(const RawRowsWriter&) = delete;
    RawRowsWriter &operator=(const RawRowsWriter&) = delete;

    void write(const Ast &value);
    void writeScalar(const ScalarAst &scalar);
    void writeObject(const ListAst &object);
    void writeArray(const ListAst &array);

private:
    void indent(const Ast *master);

private:
    RawRows &rawRows;
    int indentLevel = 0;
};

#endif // RAWROWSWRITER_H
