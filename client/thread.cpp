#include "thread.h"

Thread::Thread(QObject *parent) : QThread(parent)
{


}

Thread::~Thread(){}


void Thread::run()
{
    qRegisterMetaType<QList<int> >();

    QMutex mutex;
    mutex.lock();

    qSort(numbersList);

    mutex.unlock();
    emit sortNumbers(numbersList);


}
