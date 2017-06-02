#include <QApplication>
#include "client.h"
#include "thread.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(Client::tr("Client"));
    Client client;
    client.show();



    return app.exec();
}
