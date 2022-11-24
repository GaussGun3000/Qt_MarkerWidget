/**
*   This application is using Qt5 15.2 open source distribution version.
*/

#include "MainWindow.h"
#include <QtWidgets/QApplication>


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
