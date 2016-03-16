#include "objmodelloader.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ObjModelLoader w;
    w.show();
    return a.exec();
}
