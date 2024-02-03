#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow window;
    window.setWindowTitle("Weather");
    window.setWindowIcon(QIcon(":/images/icon.jpg"));
    window.show();

    return a.exec();
}
