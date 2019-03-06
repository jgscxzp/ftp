#include "FtpDemo.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FtpDemo w;
    w.show();

    return a.exec();
}
