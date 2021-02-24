#include "graphics.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    graphics w;
    w.show();
    return a.exec();
}
