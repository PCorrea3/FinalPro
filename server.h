#pragma once
#include <QDialog>
#include <QDebug>
#include <time.h>
#include <QList>

class QLabel;
class QPushButton;
class QTcpServer;
class QNetworkSession;

class Server : public QDialog {
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
