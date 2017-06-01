#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QDebug>

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
    void sendFortune();

private:
    QLabel *statusLabel;
    QTcpServer *tcpServer;
   // QStringList fortunes;
    QList<int> numbers;
    QNetworkSession *networkSession;
};

#endif
