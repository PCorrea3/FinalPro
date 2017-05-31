#include <QApplication>

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    //QGuiApplication::setApplicationDisplayName(NumberList::tr("Final Project"));
    // To be able to rename window
    return app.exec();
}
