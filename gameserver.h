#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#define SERVER_PORT 5010

class gameserver : public QObject
{
    Q_OBJECT
public:
    explicit gameserver(QObject *parent = nullptr);

signals:
    void ClientConnect(QTcpSocket * socClient);// сигнал что клиент подключился
    void ServerStatus(QString Status);// игнал о состоянии сервера
    void Test();

public slots:

    void startServer();// запуск сервера
    void newConnection();//подключение нового клиента

private:
    QTcpServer* server;
    bool Client = false;
};

#endif // GAMESERVER_H
