#include <QtWidgets>
#include <QtNetwork>
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include "client.h"

Client::Client(QWidget *parent)
    : QDialog(parent)
    , hostCombo(new QComboBox)
    , portLineEdit(new QLineEdit)
    , getNumberButton(new QPushButton(tr("Get Numbers")))
    , tcpSocket(new QTcpSocket(this))
    , networkSession(Q_NULLPTR)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    hostCombo->setEditable(true);
    // find out name of this machine
    QString name = QHostInfo::localHostName();
    if (!name.isEmpty()) {
        hostCombo->addItem(name);
        QString domain = QHostInfo::localDomainName();
        if (!domain.isEmpty())
            hostCombo->addItem(name + QChar('.') + domain);
    }
    if (name != QLatin1String("localhost"))
        hostCombo->addItem(QString("localhost"));
    // find out IP addresses of this machine
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // add non-localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (!ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }
    // add localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }

    portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    QLabel *hostLabel = new QLabel(tr("&Server name:"));
    hostLabel->setBuddy(hostCombo);
    QLabel *portLabel = new QLabel(tr("S&erver port:"));
    portLabel->setBuddy(portLineEdit);

    input = new QTextEdit();

    getNumberButton->setDefault(true);
    getNumberButton->setEnabled(false);

    QPushButton *quitButton = new QPushButton(tr("Quit"));

    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    buttonBox->addButton(getNumberButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    connect(hostCombo, &QComboBox::editTextChanged,
            this, &Client::enableGetNumberButton);
    connect(portLineEdit, &QLineEdit::textChanged,
            this, &Client::enableGetNumberButton);
    connect(getNumberButton, &QAbstractButton::clicked,
            this, &Client::requestNumber);

    myThread = new Thread(this);

    connect(this,SIGNAL(getValue(QList<int>)),myThread,SLOT(run(QList<int>)));
    connect(getNumberButton,SIGNAL(clicked()),this,SLOT(startThread()));
    connect(myThread,SIGNAL(sortNumbers(QList<int>)),this,SLOT(numberChanged(QList<int>)));
    connect(quitButton,SIGNAL(clicked()),this,SLOT(stop()));
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(tcpSocket, &QIODevice::readyRead, this, &Client::readNumbers);
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(tcpSocket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
            this, &Client::displayError);

    QGridLayout *mainLayout = Q_NULLPTR;
    if (QGuiApplication::styleHints()->showIsFullScreen() || QGuiApplication::styleHints()->showIsMaximized()) {
        QVBoxLayout *outerVerticalLayout = new QVBoxLayout(this);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
        QHBoxLayout *outerHorizontalLayout = new QHBoxLayout;
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        QGroupBox *groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
        mainLayout = new QGridLayout(groupBox);
        outerHorizontalLayout->addWidget(groupBox);
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        outerVerticalLayout->addLayout(outerHorizontalLayout);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));

    } else {
        mainLayout = new QGridLayout(this);
    }
    mainLayout->addWidget(hostLabel, 0, 0);
    mainLayout->addWidget(hostCombo, 0, 1);
    mainLayout->addWidget(portLabel, 1, 0);
    mainLayout->addWidget(portLineEdit, 1, 1);
    mainLayout->addWidget(input, 2, 0, 1, 2);
    mainLayout->addWidget(buttonBox, 3, 0, 1, 2);

    setWindowTitle(QGuiApplication::applicationDisplayName());
    portLineEdit->setFocus();

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Client::sessionOpened);

        getNumberButton->setEnabled(false);

        networkSession->open();
    }


    udpSocket.bind();
    connect(&udpSocket, SIGNAL(readyRead()),this,SLOT(processPendingTime()));

}

void Client::requestNumber() {
    getNumberButton->setEnabled(false);
    tcpSocket->abort();
    tcpSocket->connectToHost(hostCombo->currentText(),
                             portLineEdit->text().toInt());
}

void Client::readNumbers() {
    in.startTransaction();

    QList<int> numberslist;

    in >> numberslist;

    if (!in.commitTransaction())
        return;

    currentNumbers = numberslist;
    input->insertPlainText("Unsorted Numbers: ");

    for(int i = 0; i < 49; i++) {
        input->insertPlainText(QString::number(currentNumbers[i]));
        input->insertPlainText(" ");
    }
    input->setReadOnly(true);
    getNumberButton->setEnabled(true);
    qDebug() << currentNumbers;

    emit getValue(currentNumbers);
}

void Client::startThread() {
  input->insertPlainText("sorting operation started at ");

    processPendingDatagrams();
    myThread->start();
}

void Client::numberChanged(QList<int> numbers) {


    input->insertPlainText("\n");
    input->insertPlainText("sorting operation concluded at ");
    processPendingDatagrams();

    for(int i = 0; i < 49; i++) {
        input->insertPlainText(QString::number(numbers[i]));
        input->insertPlainText(" ");
    }
    input->insertPlainText("\n\n");
}

void Client::stop() {
    exit(1);
}

void Client::processPendingDatagrams()
{
    QByteArray time;

    do{
        time.resize(udpSocket.pendingDatagramSize());
        udpSocket.readDatagram(time.data(),time.size());
    }while(udpSocket.hasPendingDatagrams());

    QDate _date;
    QTime _time;
    QDataStream in(&time, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_0);
    in >> _date >> _time;

   input->insertPlainText(_time.currentTime().toString("hh:mm:ss.zzz"));
    input->insertPlainText(" on ");
    input->insertPlainText(_date.currentDate().toString("dd.MM.yyyy"));
    input->insertPlainText("\n");


}



void Client::displayError(QAbstractSocket::SocketError socketError) {
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Number Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Number Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the number server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Number Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }
    getNumberButton->setEnabled(true);
}

void Client::enableGetNumberButton() {
    getNumberButton->setEnabled((!networkSession || networkSession->isOpen()) &&
                                 !hostCombo->currentText().isEmpty() &&
                                 !portLineEdit->text().isEmpty());

}

void Client::sessionOpened() {
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();

   // input->setText(tr("This examples requires that you run the "
                            //"Fortune Server example as well."));
    enableGetNumberButton();
}


