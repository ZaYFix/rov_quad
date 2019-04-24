#include "rov_quad.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    rov_quad w;
    w.show();

    return a.exec();
}
