#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <QMainWindow>
#include <QList>
#include <QTimer>
#include <QLabel>
#include <QPixmap>
#include <QGroupBox>
#include <QFormLayout>
#include <QLayout>
#include <QRadioButton>
#include <QPushButton>
#include <QStateMachine>
#include<QThread>
#include "gameserver.h"
#include "connect_dialog.h"
#include "basefield.h"

#define RunningTime         10
//#define
//#define CHANGE_PLAYER       2
//#define RUNNING_TIME        3

enum
{
    SET_NAME   =         1,
    CHANGE_PLAYER,
    RUNNING_TIME,
    PREPARED,
    SHOT,
    MISS,
    HIT,
    KILLED,
    OVER
};
enum
{
    UP = 0,
    DOWN,
    LEFT,
    RIGHT
};

struct strShip
{
    QRadioButton* rbShip;
    QLabel* lShip;
    QRadioButton* rbCourse;
    int* amountShip;
    int Course;
    int size;
};

namespace Ui {
class game_map;
}

class game_map : public QMainWindow
{
    Q_OBJECT

public:
    explicit game_map(QWidget *parent = nullptr);
    ~game_map();
private:
    Ui::game_map *ui;
    QStateMachine machine;//State Machine
    gameserver * server;// server socket pointer
    QTcpSocket * Client;// pointer to client socket
    bool bClientConnect = false;// is the client connected
    QString name;//Username
    QString Player2Name;//subconnon player name
    QString IPserver;//IP server
    connect_dialog * startdlg;// start dialog for creating connections
    QByteArray data;// incoming message
    bool ActivePlayer = false;//Does the move fit the user
    QTimer* timerRunningTime;// end timer
    int SecondsLeft = 0;//how many seconds before the end of the course
    QList<QLabel *> LabelsCoordinate;//user field coordinate list
    QList<BaseField *> LabelsField;//user playlist
    bool LabelsFieldClicked = true;//is user card active
    QList<deck*> listDeck;//list of decks created
    int CurrentIDShip = 0;// current ship id
    bool PreparedUser = false;// the user has set the ships and is ready to start the game
    bool PreparedUserPlayer2 = false;// The 2nd player set the ships and is ready to start the game
    bool GameOver = false;// Game Over

    QList<QLabel *> LabelsCoordinatePlayer2;//Player2 field coordinate list
    QList<BaseField *> LabelsFieldPlayer2;//a list of the playing field of user Player2
    QList<deck*> listDeckPlayer2;//list of created decks Player2
/* graphic controls for creating and arranging ships*/
    QGroupBox* groboxShip;
    QFormLayout* flShip;
    QRadioButton* rbShip1;
    QRadioButton* rbShip2;
    QRadioButton* rbShip3;
    QRadioButton* rbShip4;
    QLabel* lShip1;
    QLabel* lShip2;
    QLabel* lShip3;
    QLabel* lShip4;
    QHBoxLayout* hblCourse;

    QRadioButton* rbUP;
    QRadioButton* rbDOWN;
    QRadioButton* rbLEFT;
    QRadioButton* rbRIGHT;
    QGroupBox* groboxCourse;
/**************how many ships and what type can be created***********************************************/
    int amountShip4 = 1;
    int amountShip3 = 2;
    int amountShip2 = 3;
    int amountShip1 = 4;

    strShip CurentShip; //structure for storing selected ship parameters

    QPushButton* StartGame;//ready button




    void ini();// object initialization
    void initStateMachine();//State Machine initialization
    void writeMessage(QByteArray mess);//sending messages on a socket
    QByteArray CreateMessageName(QByteArray pName);//composing a message to transmit a username
    QByteArray CreateMessagePrepared();//composing a message to convey player readiness
    QByteArray CreateMessageSecondsLeft(int pSeconds);//composing a message to transmit the remaining time for the user’s move
    QByteArray CreateMessageChangePlayer();//composing a message to pass the move to the second player
    QByteArray CreateMessageShot(int px, int py); //composing a message to transmit the coordinates of the shot
    QByteArray CreateMessageMiss(int px, int py); //composing a response message "miss"
    QByteArray CreateMessageHit(int px, int py); //composing a response message "hit"
    QByteArray CreateMessageKilled(int px, int py); //composing a response message "killed"
    QByteArray CreateMessageGameOver(int px, int py); //composing a response message "Game over"
    void ParserMessage(QByteArray baParam);//recognition of incoming messages

    void CreateGUIShipLayout();//creating a user interface for arranging ships
    BaseField * SearchField(QList<BaseField *> list, int px, int py);// search field by coordinates
    void CreateShip(int Xstart, int Ystart, int SizsShip, int course);// ship making
    bool CheckCreateShip(int Xstart, int Ystart, int SizsShip, int course);//check whether it is possible to create a ship at the given coordinates
    bool CheckNeighboringShips(int Xstart, int Ystart, int SizsShip, int course);//check whether it is possible to create a ship at the given coordinates
    bool CheckNeighboringShips(int Xstart, int Ystart);//check whether it is possible to create a ship at the given coordinates
    void CreateGUIStart();// creates a GUI Prepared
    bool CheckFinishCreateShip();// check whether all ships are created
    void DisconnectRadioButton();// disconnects slots from the signal to remove RadioButton
    void DeletionGUIShipLayout();//removal of the user interface for the arrangement of ships
    bool CheckPrepared(); // check if players are ready to start the game


    void SetPlayer2Name(QByteArray pName);//naming a second player
    void CreateMapPlayer2();// creating a second player map
    bool CheckShot(int px, int py);//check the field where the second player shot
    bool CheckShot(QByteArray px, QByteArray py);//check the field where the second player shot
    bool CheckDamagedField(int px, int py); //check whether the shot was fired in the field
    void miss(QByteArray px, QByteArray py);//filling the playing field if the user missed
    void hit(QByteArray px, QByteArray py);//filling the playing field if the user hits
    bool CheckKilled(BaseField * Field);//check if the ship was killed when hit
    void killed(QByteArray px, QByteArray py);//filling the playing field if the user killed the ship
    void killed(int px, int py);//filling the playing field if the user killed the ship
    bool CheckAllShipsKilled();//check if all ships are killed


    void RefreshSecondsLeft(QByteArray pSeconds);//display of the remaining seconds until the end of the move
    void ChangePlayer();// player change message processing
    void BeginOfTurn();// start move user
    void EndOfTurn(); // end move user






signals:
    void startServer();// signal to start the server
    void BeginOT();// signals start move user
    void EndOT(); // signals end move user
    void GOver(); // signals end move user
public slots:


    void CreateServer();// server creation

    void ServerStatus(QString Status);// server status
    void Connect(QString IPserver);//client socket connection over IP server
    void Reconnecting();// reconnecting to server
    void ClientConnect(QTcpSocket * socClient);// external client connection
    void UserName(QString name);//username assignment
    void readyRead();// read message from socket
    void updateTime();//timer update
    void SocketConnection();//socket connected to server
    void connectionErrors(QAbstractSocket::SocketError err);// error connecting to server

    void ClickableLabel(int px, int py);
   //************************************************************
    void rbShip1_clicked();
    void rbShip2_clicked();
    void rbShip3_clicked();
    void rbShip4_clicked();

    void rbUP_clicked();
    void rbDOWN_clicked();
    void rbLEFT_clicked();
    void rbRIGHT_clicked();
//*****************************************************************************
    void StartGame_clicked();//puts the user in a ready state

    void shot(int px, int py);// shot on the field of the second player

private slots:
    void on_pbConnect_clicked();
};

#endif // GAME_MAP_H
