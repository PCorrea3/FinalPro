#ifndef TIMER_H
#define TIMER_H
#include <QWidget>
#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QDataStream>
#include <QDebug>
class Timer : public QWidget
{
    Q_OBJECT
public:
    Timer();
     void sendTime();

private slots:


private:
   QUdpSocket *udpSocket;

};

#endif // TIMER_H
