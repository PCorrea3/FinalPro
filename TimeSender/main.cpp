#include <QApplication>
#include "timer.h"


int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    Timer timer;
    timer.show();

    return app.exec();

}
