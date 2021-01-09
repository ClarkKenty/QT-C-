#include "login.h"
#include "application.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;
    w.setWindowTitle("登录");
    w.show();
    return a.exec();
}
