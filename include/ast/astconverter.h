#ifndef ASTCONVERTER_H
#define ASTCONVERTER_H

#include "ast/ast.h"
#include "ast/rootast.h"
#include "core/hammer.h"
#include "rapidjson/document.h"
#include <memory>

class AstConverter
{
public:
    // typedef typenaRawRowsme rapidjson::UTF8<> Ch; // src encoding
    typedef char Ch; // src encoding

    AstConverter(RootAst &root);
    void convert(const rapidjson::Document &d);

    bool Null();
    bool Bool(bool b);
    bool Int(int i);
    bool Uint(unsigned i);
    bool Int64(int64_t i);
    bool Uint64(uint64_t i);
    bool Double(double d);
    bool String(const Ch* str, rapidjson::SizeType length, bool copy);
    bool StartObject();
    bool Key(const Ch* str, rapidjson::SizeType length, bool copy);
    bool EndObject(rapidjson::SizeType memberCount);
    bool StartArray();
    bool EndArray(rapidjson::SizeType elementCount);

private:
    void convertScalar(const std::string &text);

private:
    RootAst &root;
    Ast *outer = &root; // parent of insertee
    size_t inner = 0; // the position to insert
};

#endif // ASTCONVERTER_H
