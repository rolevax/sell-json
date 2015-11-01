#ifndef MODE_H
#define MODE_H

class Mode
{
public:
    Mode();
    virtual void execute(int key) = 0;
};

#endif // MODE_H
