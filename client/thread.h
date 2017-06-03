#ifndef THREAD_H
#define THREAD_H
#include <QThread>
#include <QDebug>
#include <algorithm>

class Client;
class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(QObject *parent = 0);
    ~Thread();
    void run();
    QList<int> numbersList;
public slots:

signals:
    void sortNumbers(QList<int>);
private:

};

#endif // THREAD_H
