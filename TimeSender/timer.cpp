#include "timer.h"

Timer::Timer()
{

}
Timer::~Timer(){}
void Timer::sendTime()
{
    QByteArray time;
    QDataStream out (&time, QIODevice::WriteOnly);
    out << QDate::currentDate().toString("dd.MM.yyyy") << QTime::currentTime().toString("hh:mm:ss.zzz");

    udpSocket.writeDatagram(time, QHostAddress::LocalHost, tcpServer->serverPort());


}
