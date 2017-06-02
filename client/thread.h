#ifndef THREAD_H
#define THREAD_H
#include <QThread>
#include <QDebug>
class Thread : public QThread
{
public:
    Thread();
    ~Thread();
    void run();
    QList<int>* numberslist;
public slots:

private:

};

#endif // THREAD_H
