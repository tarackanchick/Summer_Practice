#include <QApplication>

#include "mainwindow.h"
#include "controller.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    Controller controller(&window);

    window.show();

    return app.exec();
}