#ifndef TIMER_H
#define TIMER_H
#include <QWidget>
#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QDataStream>
#include <QTcpServer>
class Timer : public QWidget
{
    Q_OBJECT
public:
    Timer();
    ~Timer();

private slots:
    void sendTime();

private:
   QUdpSocket udpSocket;
   QTcpServer *tcpServer;
    QTimer timer;
};

#endif // TIMER_H
