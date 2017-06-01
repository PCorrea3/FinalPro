#pragma once

#include <QDialog>
#include <QTcpSocket>
#include <QDataStream>
#include <QTextEdit>
#include <QString>

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

private slots:
    void requestNumber();
    void readNumbers();
    void displayError(QAbstractSocket::SocketError socketError);
    void enableGetNumberButton();
    void sessionOpened();
    //void sendNumbers();
private:
    QComboBox *hostCombo;
    QLineEdit *portLineEdit;
    QLabel *statusLabel;
    QPushButton *getNumberButton;

    QTcpSocket *tcpSocket;
    QDataStream in;
    QString currentFortune;

    QNetworkSession *networkSession;

//    QLineEdit *input;
//    QPushButton *inputFortune;
//    QByteArray *fortune;
};
