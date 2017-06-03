#include "thread.h"

Thread::Thread(QObject *parent) : QThread(parent)
{

}

Thread::~Thread(){}

void Thread::run()
{
    QMutex mutex;
    mutex.lock();

    qSort(numbersList);

    mutex.unlock();
    emit sortNumbers(numbersList);



   // qDebug() << numbersList;




}
