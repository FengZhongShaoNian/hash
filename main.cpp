#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Hash");
    w.setFixedSize(508, 363);
    Qt::WindowFlags flags = w.windowFlags();
    flags &= ~Qt::WindowMinMaxButtonsHint;
    flags |= Qt::WindowMinimizeButtonHint;
    w.setWindowFlags(flags);
    w.show();

    return a.exec();
}
