#ifndef THREAD_H
#define THREAD_H
#include <QThread>
#include <QDebug>
#include <algorithm>
#include <QMetaType>


Q_DECLARE_METATYPE(QList<int>)


class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(QObject *parent = 0);
    ~Thread();
    void run();


    QList<int> numbersList;



signals:
    void sortNumbers(QList<int>);
    QList<int> getValue();
private:

};

#endif // THREAD_H
