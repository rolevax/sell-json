#include "mode/viewmode.h"
#include <QDebug>

ViewMode::ViewMode()
{

}

void ViewMode::execute(int key)
{
    if (key == 67) { // TODO: de-magic
        qDebug() << "ViewMode: haha, C pressed!";
    }
}

