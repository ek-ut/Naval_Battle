#include "gameserver.h"
gameserver::gameserver(QObject *parent) : QObject(parent)
{

}

void gameserver::startServer()
{
    QString s ;
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    if(!server->listen(QHostAddress::Any, SERVER_PORT))
    {
         s = server->errorString();


    }else
    {
        s  ="Start server";
    }
    ServerStatus(s);
    //Test();
}
void gameserver::newConnection()
{
    if(!Client)
    {
        QTcpSocket* ClientSocet = server->nextPendingConnection();
        ClientConnect(ClientSocet);
        Client = true;
    }
}
