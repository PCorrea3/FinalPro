#include "timer.h"

Timer::Timer()
{

    udpSocket.bind(QHostAddress::LocalHost,1234);

}

void Timer::sendTime()
{
    QByteArray time;
    QDataStream out (&time, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    //out << QDate::currentDate().toString("dd.MM.yyyy") << QTime::currentTime().toString("hh:mm:ss.zzz");
out << QDateTime::currentDateTime();
    udpSocket.writeDatagram(time, QHostAddress::LocalHost, 1234);
}



