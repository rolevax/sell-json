#ifndef MODE_H
#define MODE_H


class Mode
{
public:
    enum class Type
    {
        VIEW, MENU, INPUT_STRING, INPUT_NUMBER
    };

    Mode(Type type);
    // TODO: consider delete default copy & op=

    Type getType() const;

private:
    Type type;
};

#endif // MODE_H
