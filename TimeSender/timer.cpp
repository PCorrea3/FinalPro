#include "timer.h"

Timer::Timer()
{
    connect(&timer, SIGNAL(timeout()),this,SLOT(sendTime()));

    timer.start();
}
Timer::~Timer(){}
void Timer::sendTime()
{
    QByteArray time;
    QDataStream out (&time, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << QDate::currentDate() << QTime::currentTime();

    udpSocket.writeDatagram(time, QHostAddress::LocalHost, tcpServer->serverPort());


}
