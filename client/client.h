#pragma once

#include <QDialog>
#include <QTcpSocket>
#include <QDataStream>
#include <QTextEdit>
#include <QString>
#include <QMetaType>
#include "thread.h"



class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
class QNetworkSession;

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = Q_NULLPTR);

    Thread *myThread;


private slots:
    void requestNumber();
    void readNumbers();
    void displayError(QAbstractSocket::SocketError socketError);
    void enableGetNumberButton();
    void sessionOpened();

private:
    QComboBox *hostCombo;
    QLineEdit *portLineEdit;
    QLabel *statusLabel;
    QTextEdit *input;
    QPushButton *getNumberButton;

    QTcpSocket *tcpSocket;
    QDataStream in;
    QString currentFortune;
    QList<int> currentNumbers;
    QByteArray *numbers;

    QNetworkSession *networkSession;
    QByteArray *fortune;


public slots:
   void numberChanged(QList<int>);
   void startThread();
   QList<int> getValue();
signals:



};
