//
// Created by William Scheirey on 8/20/24.
//

#ifndef GS_BACKEND_2024_2025_WEBSERVER_H
#define GS_BACKEND_2024_2025_WEBSERVER_H

#include <QObject>
#include <QWebSocket>
#include <QWebSocketServer>

class WebSocket : public QObject
{
    Q_OBJECT

public:
    QWebSocket *socket;

    explicit WebSocket(QWebSocket *socket) : socket(socket)
    {
        connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this,
                SLOT(stateChanged(QAbstractSocket::SocketState)));


    }

public slots:

            void stateChanged(QAbstractSocket::SocketState socketState)
    {
        emit stateChanged(this, socketState);
    };

    void sendData(const QString &data)
    {
        socket->sendTextMessage(data);
    };

    signals:

            void stateChanged(WebSocket *socket, QAbstractSocket::SocketState);
};

class WebServer : public QObject
{
    Q_OBJECT

public:
    WebServer(int port, QObject *parent = nullptr);

    int port;

    void broadcast(const QString &str);

private:
    QWebSocketServer server;
    QList<WebSocket *> clients;


public slots:

            static void acceptError(QAbstractSocket::SocketError socketError);

    void alertReceived(QSsl::AlertLevel level, QSsl::AlertType type, const QString &description);

    void alertSent(QSsl::AlertLevel level, QSsl::AlertType type, const QString &description);

    void closed();

    void newConnection();

    void serverError(QWebSocketProtocol::CloseCode closeCode);

    void clientStateChanged(WebSocket *socket, const QAbstractSocket::SocketState &state);
};

#endif //GS_BACKEND_2024_2025_WEBSERVER_H
