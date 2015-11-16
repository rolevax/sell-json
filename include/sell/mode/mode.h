#ifndef MODE_H
#define MODE_H

class Doc;

class Mode
{
public:
    enum class Type
    {
        VIEW, MENU, INPUT_STRING, INPUT_NUMBER
    };

    Mode(Type type, Doc &doc);
    Mode(const Mode& copy) = delete;
    Mode &operator=(const Mode& assign) = delete;

    Type getType() const;

private:
    Type type;
    Doc &doc;
};

#endif // MODE_H
