#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QDebug>
#include <time.h>


class QLabel;
class QPushButton;
class QTcpServer;
class QNetworkSession;
#include <QList>

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = Q_NULLPTR);

private slots:
    void sessionOpened();
    void sendNumbers();
    void quit();
private:
    QLabel *statusLabel;
    QTcpServer *tcpServer;
    QList<int> numbers;
    QNetworkSession *networkSession;
};

#endif
