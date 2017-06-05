#include <QtWidgets>
#include <QtNetwork>

#include <stdio.h>
#include <stdlib.h>
#include <QThread>

#include "server.h"

Server::Server(QWidget *parent)
    : QDialog(parent)
    , statusLabel(new QLabel)
    , tcpServer(Q_NULLPTR)
    , networkSession(0)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    statusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

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
        connect(networkSession, &QNetworkSession::opened, this, &Server::sessionOpened);

        statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    } else {
        sessionOpened();
    }



    for(int i =0; i < 49; i++)
    {

      numbers.append(qrand()%(1001 - 0) + 0);

    }

        QPushButton *quitButton = new QPushButton(tr("Quit"));
        quitButton->setAutoDefault(false);
        connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
        connect(tcpServer, &QTcpServer::newConnection, this, &Server::sendNumbers);

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addStretch(1);
        buttonLayout->addWidget(quitButton);
        buttonLayout->addStretch(1);

        QVBoxLayout *mainLayout = Q_NULLPTR;
        if (QGuiApplication::styleHints()->showIsFullScreen() || QGuiApplication::styleHints()->showIsMaximized()) {
            QVBoxLayout *outerVerticalLayout = new QVBoxLayout(this);
            outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
            QHBoxLayout *outerHorizontalLayout = new QHBoxLayout;
            outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
            QGroupBox *groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
            mainLayout = new QVBoxLayout(groupBox);
            outerHorizontalLayout->addWidget(groupBox);
            outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
            outerVerticalLayout->addLayout(outerHorizontalLayout);
            outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
        } else {
            mainLayout = new QVBoxLayout(this);
        }

        mainLayout->addWidget(statusLabel);
        mainLayout->addLayout(buttonLayout);

        setWindowTitle(QGuiApplication::applicationDisplayName());

        connect(quitButton,SIGNAL(clicked()),this,SLOT(quit()));
}

void Server::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
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
    }

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen()) {
        QMessageBox::critical(this, tr("Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return;
    }
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Run the Fortune Client example now.")
                         .arg(ipAddress).arg(tcpServer->serverPort()));
}

void Server::sendNumbers()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    //endless loop
    //for(;;)
    for(int i = 0;i < 3; i++)
    {

    out << numbers;
    qDebug() << numbers;

    QApplication::processEvents();
}
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected,
            clientConnection, &QObject::deleteLater);

    clientConnection->write(block);
    clientConnection->disconnectFromHost();

}

void Server::quit()
{
    exit(1);
}
