#include <QApplication>

#include "GUI/MainWidget.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    MainWidget widget;
    widget.show();

    return QApplication::exec();
}
