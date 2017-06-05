#include "thread.h"

Thread::Thread(QObject *parent) : QThread(parent)
{


}

Thread::~Thread(){}



void Thread::run()
{
    qRegisterMetaType<QList<int> >();
     numbersList = getValue();
    QMutex mutex;
    mutex.lock();

    qSort(numbersList);

    mutex.unlock();
    emit sortNumbers(numbersList);


}
