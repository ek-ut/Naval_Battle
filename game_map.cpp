#include "game_map.h"
#include "ui_game_map.h"

game_map::game_map(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::game_map)
{
    ui->setupUi(this);
    ini();
}

game_map::~game_map()
{
    delete ui;
}

void game_map::ini()//object initialization
{
    //pixSea = QPixmap(":/images/sea.png");


    timerRunningTime = new QTimer();
    timerRunningTime->setInterval(1000);
    connect(timerRunningTime, SIGNAL(timeout()), this, SLOT(updateTime()));

    initStateMachine();
}


void game_map::initStateMachine()//State Machine initialization
{

    QLabel * CerentLabel;
    BaseField * CerentBFU;
    QState *State1 = new QState();//move user
    QState *State2 = new QState();//move Player2
    QState *State3 = new QState();//Game over
    for(int i = 0; i < 20; i++)
    {
        CerentLabel = new QLabel();
        LabelsCoordinate.append(CerentLabel);
        State1->assignProperty(CerentLabel, "visible", true);
        State2->assignProperty(CerentLabel, "visible", false);
        State3->assignProperty(CerentLabel, "visible", true);
        State3->assignProperty(CerentLabel, "enabled", false);

        CerentLabel = new QLabel();
        LabelsCoordinatePlayer2.append(CerentLabel);
        State1->assignProperty(CerentLabel, "enabled", false);
        State2->assignProperty(CerentLabel, "enabled", true);
        State3->assignProperty(CerentLabel, "visible", true);
        State3->assignProperty(CerentLabel, "enabled", false);
    }

    for(int i = 0; i < 100; i ++)
    {
        CerentBFU = new BaseField();
        LabelsField.append(CerentBFU);
        State1->assignProperty(CerentBFU, "visible", true);
        State2->assignProperty(CerentBFU, "visible", false);
        State3->assignProperty(CerentBFU, "visible", true);


        CerentBFU = new BaseField();
        LabelsFieldPlayer2.append(CerentBFU);
    }
    State1->assignProperty(ui->lUserName, "visible", true);
    State2->assignProperty(ui->lUserName, "visible", false);
    State3->assignProperty(ui->lUserName, "visible", true);

    StartGame = new QPushButton("Prepared...");

    State1->addTransition(this, SIGNAL(BeginOT()), State2);
    State2->addTransition(this, SIGNAL(EndOT()), State1);
    State1->addTransition(this, SIGNAL(GOver()), State3);
    State2->addTransition(this, SIGNAL(GOver()), State3);

    machine.addState(State1);
    machine.addState(State2);
    machine.addState(State3);
    machine.setInitialState(State1);

}
void game_map::writeMessage(QByteArray mess)//sending messages on a socket
{
    if(!GameOver)
    {
        Client->write(mess);
        QThread::msleep(100);
    }

}

QByteArray game_map::CreateMessageName(QByteArray pName)//composing a message to transmit a username
{
    QByteArray baReturn;
    baReturn = QByteArray::number(SET_NAME,10) + ";" + pName;
    return baReturn;
}

QByteArray game_map::CreateMessagePrepared()//composing a message to convey player readiness
{
    QByteArray baReturn;
    baReturn = QByteArray::number(PREPARED,10);
    return baReturn;
}

QByteArray game_map::CreateMessageSecondsLeft(int pSeconds)//composing a message to transmit the remaining time for the user’s move
{
    QByteArray baReturn;
    baReturn = QByteArray::number(RUNNING_TIME,10) + ";" + QByteArray::number(pSeconds,10);
    return baReturn;
}

QByteArray game_map::CreateMessageChangePlayer()//composing a message to pass the move to the second player
{
    QByteArray baReturn;
    baReturn = baReturn.number(CHANGE_PLAYER,10);
    return baReturn;
}

QByteArray game_map::CreateMessageShot(int px, int py)//composing a message to transmit the coordinates of the shot
{
    QByteArray baReturn;
    baReturn = QByteArray::number(SHOT,10) + ";" + QByteArray::number(px,10) + ";" + QByteArray::number(py,10);
    return baReturn;

}

QByteArray game_map::CreateMessageMiss(int px, int py)//composing a response message "miss"
{
    QByteArray baReturn;
    baReturn = QByteArray::number(MISS,10) + ";" + QByteArray::number(px,10) + ";" + QByteArray::number(py,10);
    return baReturn;
}

QByteArray game_map::CreateMessageHit(int px, int py)//composing a response message "hit"
{
    QByteArray baReturn;
    baReturn = QByteArray::number(HIT,10) + ";" + QByteArray::number(px,10) + ";" + QByteArray::number(py,10);
    return baReturn;
}

QByteArray game_map::CreateMessageKilled(int px, int py) //composing a response message "killed"
{
    QByteArray baReturn;
    baReturn = QByteArray::number(KILLED,10) + ";" + QByteArray::number(px,10) + ";" + QByteArray::number(py,10);
    return baReturn;
}

QByteArray game_map::CreateMessageGameOver(int px, int py) //composing a response message "Game over"
{
    QByteArray baReturn;
    baReturn = QByteArray::number(OVER,10) + ";" + QByteArray::number(px,10) + ";" + QByteArray::number(py,10);
    return baReturn;
}

void game_map::ParserMessage(QByteArray baParam)//recognition of incoming messages
{
    QByteArray Com;
    int iCom;
    QList<QByteArray> list = baParam.split(';');
    Com = list.at(0);
    iCom = Com.toInt();
    switch (iCom) {
    case SET_NAME:
        SetPlayer2Name(list.at(1));
        break;
    case CHANGE_PLAYER:
        ChangePlayer();
        break;
    case RUNNING_TIME:
        RefreshSecondsLeft(list.at(1));
        break;
    case PREPARED:
        PreparedUserPlayer2 = true;
        if(CheckPrepared())
        {
            CreateMapPlayer2();
        }
        break;
    case SHOT:
        CheckShot(list.at(1), list.at(2));
        break;
    case MISS:
            miss(list.at(1), list.at(2));
        break;
    case HIT:
        hit(list.at(1), list.at(2));
        break;
    case KILLED:
        killed(list.at(1), list.at(2));
        break;
    case OVER:
        GOver();
        GameOver = true;
        killed(list.at(1), list.at(2));
        ui->lStatus->setText("Game Over \n\n Winner: " + name);
        break;
    default:
        break;
    }
}

void game_map::CreateGUIShipLayout()//creating a user interface for arranging ships
{

    machine.start();
    int sizeLebl = LabelsCoordinate.size() -1;
    int CurentBaseField = 0;
    for(int i = 0; i < 10; i++)
    {
        QLabel * CerentLabel = LabelsCoordinate.at(i);
        char a = (char)(i + 97);
        CerentLabel->setAlignment(Qt::AlignCenter);
        CerentLabel->setText((QString)a);
        ui->glUserMap->addWidget(CerentLabel,0,i+1,1,1);

        CerentLabel = LabelsCoordinate.at(sizeLebl - i);
        CerentLabel->setText(QString::number(i+1, 10));
        ui->glUserMap->addWidget(CerentLabel,i+1,0,1,1);
    }

    for(int i = 1; i <= 10; i++)
        for(int j = 1; j <= 10; j++ )
        {
            BaseField * CerentLabel = LabelsField.at(CurentBaseField);
            CerentLabel->SetCoordinate(j,i);
            connect(CerentLabel, SIGNAL(clickedField(int, int)), this, SLOT(ClickableLabel(int, int)));
            ui->glUserMap->addWidget(CerentLabel,i,j,1,1);
            CurentBaseField++;
        }


    groboxShip = new QGroupBox();
    ui->glClientMap->addWidget(groboxShip, 0,0, 5,5);
    groboxCourse = new QGroupBox();
    ui->glClientMap->addWidget(groboxCourse, 5,0, 5,5);

    flShip = new QFormLayout();
    rbShip4 = new QRadioButton("4 deck");
    lShip4 = new QLabel("not exposed ships " + QString::number(amountShip4));
    flShip->addRow(rbShip4, lShip4);
    rbShip3 = new QRadioButton("3 deck");
    lShip3 = new QLabel("not exposed ships "+ QString::number(amountShip3));
    flShip->addRow(rbShip3, lShip3);
    rbShip2 = new QRadioButton("2 deck");
    lShip2 = new QLabel("not exposed ships "+ QString::number(amountShip2));
    flShip->addRow(rbShip2, lShip2);
    rbShip1 = new QRadioButton("1 deck");
    lShip1 = new QLabel("not exposed ships "+ QString::number(amountShip1));
    flShip->addRow(rbShip1, lShip1);
    groboxShip->setLayout(flShip);

    rbShip4->setChecked(true);

    rbUP = new QRadioButton("UP");
    rbDOWN = new QRadioButton("DOWN");
    rbLEFT = new QRadioButton("LEFT");
    rbRIGHT = new QRadioButton("RIGHT");
    hblCourse = new QHBoxLayout();
   hblCourse->addWidget(rbUP);
   hblCourse->addWidget(rbDOWN);
   hblCourse->addWidget(rbLEFT);
   hblCourse->addWidget(rbRIGHT);

   groboxCourse->setLayout(hblCourse);
   rbLEFT->setChecked(true);
   CurentShip.amountShip = &amountShip4;
   CurentShip.rbShip = rbShip4;
   CurentShip.lShip = lShip4;
   CurentShip.size = 4;
   CurentShip.rbCourse = rbLEFT;
   CurentShip.Course = LEFT;


   connect(rbShip4, SIGNAL(clicked()), this, SLOT(rbShip4_clicked()));
   connect(rbShip3, SIGNAL(clicked()), this, SLOT(rbShip3_clicked()));
   connect(rbShip2, SIGNAL(clicked()), this, SLOT(rbShip2_clicked()));
   connect(rbShip1, SIGNAL(clicked()), this, SLOT(rbShip1_clicked()));

   connect(rbUP, SIGNAL(clicked()), this, SLOT(rbUP_clicked()));
   connect(rbDOWN, SIGNAL(clicked()), this, SLOT(rbDOWN_clicked()));
   connect(rbLEFT, SIGNAL(clicked()), this, SLOT(rbLEFT_clicked()));
   connect(rbRIGHT, SIGNAL(clicked()), this, SLOT(rbRIGHT_clicked()));

}

BaseField * game_map::SearchField(QList<BaseField *> list, int px, int py)// search field by coordinates
{
    BaseField* returnField = NULL;
    int iSize = list.size();
    int x, y;
    for(int i = 0; i < iSize; i++)
    {
        returnField = list.at(i);
        returnField->GetCoordinate(&x, &y);
        if(x==px && y == py)
        {
            break;
        }else
        {
           returnField = NULL;
        }
    }
    return returnField;
}

void game_map::CreateShip(int Xstart, int Ystart, int SizsShip, int course)// ship making
{
    BaseField* Temp;
    for(int i = 0; i < SizsShip; i++)
    {
        switch(course)
        {
        case UP:
            Temp = SearchField(LabelsField,Xstart,Ystart-i);
            break;
        case DOWN:
            Temp = SearchField(LabelsField, Xstart,Ystart+i);
            break;
        case LEFT:
            Temp = SearchField(LabelsField, Xstart-i,Ystart);
            break;
        case RIGHT:
            Temp = SearchField(LabelsField, Xstart+i,Ystart);
            break;

        }
        deck* CurrentDeck = new deck();
        CurrentDeck->SetShipID(CurrentIDShip);
        listDeck.append(CurrentDeck);
        Temp->AddDeck(CurrentDeck);
    }
    CurrentIDShip++;
    *CurentShip.amountShip = *CurentShip.amountShip - 1;
    CurentShip.lShip->setText("not exposed ships " + QString::number(*CurentShip.amountShip));
    if(*CurentShip.amountShip < 1)
    {
       CurentShip.rbShip->setEnabled(false);
       CurentShip.lShip->setEnabled(false);
    }
}

bool game_map::CheckCreateShip(int Xstart, int Ystart, int SizsShip, int course)//check whether it is possible to create a ship at the given coordinates
{
    if(*CurentShip.amountShip < 1)
    {
        return false;
    }
    bool checkReturn = false;
    int sizep = SizsShip - 1;
    switch(course)
    {
    case UP:
        if((Ystart-sizep) > 0)
        {
            checkReturn = true;
        }
        break;
    case DOWN:
        if((Ystart + sizep) <= 10)
        {
            checkReturn = true;
        }
        break;
    case LEFT:
        if((Xstart-sizep) > 0)
        {
            checkReturn = true;
        }
        break;
    case RIGHT:
        if((Xstart + sizep) <= 10)
        {
            checkReturn = true;
        }
        break;

    }
    if(checkReturn)
    {
        checkReturn = CheckNeighboringShips(Xstart,Ystart, SizsShip, course);
    }
    return checkReturn;
}

bool game_map::CheckNeighboringShips(int Xstart, int Ystart, int SizsShip, int course)//check whether it is possible to create a ship at the given coordinates
{
    bool checkReturn = true;
    for(int i = 0; i < SizsShip; i++)
    {
        switch(course)
        {
        case UP:
            checkReturn = CheckNeighboringShips(Xstart,Ystart-i);
            break;
        case DOWN:
            checkReturn = CheckNeighboringShips(Xstart,Ystart+i);
            break;
        case LEFT:
            checkReturn = CheckNeighboringShips(Xstart-i,Ystart);
            break;
        case RIGHT:
            checkReturn = CheckNeighboringShips(Xstart+i,Ystart);
            break;

        }
        if(!checkReturn)
        {
            break;
        }

    }
    return checkReturn;
}

bool game_map::CheckNeighboringShips(int Xstart, int Ystart)//check whether it is possible to create a ship at the given coordinates
{
    bool checkReturn = true;
    int x,y;
    deck* Temp;
    int sizeDack = listDeck.size();
    for(int i=Xstart - 1; i <= Xstart + 1; i++ )
        for(int j = Ystart-1; j <= Ystart + 1; j++)
        {
            for(int d=0; d<sizeDack; d++)
            {
                Temp = listDeck.at(d);
                Temp->GetCoordinate(&x , &y);
                if(x==i && y==j)
                {
                   checkReturn = false;
                }
            }

        }
    return checkReturn;
}

void game_map::CreateGUIStart()// creates a GUI Prepared
{
   // StartGame = new QPushButton("Prepared...");
    connect(StartGame, SIGNAL(clicked()), this, SLOT(StartGame_clicked()));
    ui->glClientMap->addWidget(StartGame);
}

bool game_map::CheckFinishCreateShip()// check whether all ships are created
{
    if(amountShip1<1 && amountShip2<1 && amountShip3<1 && amountShip4<1)
    {
        return true;
    }
    return false;
}

void game_map::DisconnectRadioButton()// disconnects slots from the signal to remove RadioButton
{
    disconnect(rbShip4, SIGNAL(clicked()), this, SLOT(rbShip4_clicked()));
    disconnect(rbShip3, SIGNAL(clicked()), this, SLOT(rbShip3_clicked()));
    disconnect(rbShip2, SIGNAL(clicked()), this, SLOT(rbShip2_clicked()));
    disconnect(rbShip1, SIGNAL(clicked()), this, SLOT(rbShip1_clicked()));

    disconnect(rbUP, SIGNAL(clicked()), this, SLOT(rbUP_clicked()));
    disconnect(rbDOWN, SIGNAL(clicked()), this, SLOT(rbDOWN_clicked()));
    disconnect(rbLEFT, SIGNAL(clicked()), this, SLOT(rbLEFT_clicked()));
    disconnect(rbRIGHT, SIGNAL(clicked()), this, SLOT(rbRIGHT_clicked()));
}

void game_map::DeletionGUIShipLayout()//removal of the user interface for the arrangement of ships
{
    delete rbShip1;
    delete rbShip2;
    delete rbShip3;
    delete rbShip4;
    delete lShip1;
    delete lShip2;
    delete lShip3;
    delete lShip4;

    delete rbUP;
    delete rbDOWN;
    delete rbLEFT;
    delete rbRIGHT;

    delete hblCourse;
    delete flShip;
    delete groboxShip;
    delete groboxCourse;
}

bool game_map::CheckPrepared()// check if players are ready to start the game
{
    return PreparedUser && PreparedUserPlayer2;

}

void game_map::SetPlayer2Name(QByteArray pName)//naming a second player
{
    Player2Name = pName;
    ui->lClientName->setText(pName);
}

void game_map::CreateMapPlayer2()// creating a second player map
{
    int sizeLebl = LabelsCoordinatePlayer2.size() -1;
    int CurentBaseField = 0;
    for(int i = 0; i < 10; i++)
    {
        QLabel * CerentLabel = LabelsCoordinatePlayer2.at(i);

        char a = (char)(i + 97);
        CerentLabel->setAlignment(Qt::AlignCenter);
        CerentLabel->setText((QString)a);
        //LabelsCoordinatePlayer2.append(CerentLabel);
        ui->glClientMap->addWidget(CerentLabel,0,i+1,1,1);

        CerentLabel = LabelsCoordinatePlayer2.at(sizeLebl -i);
        CerentLabel->setText(QString::number(i+1, 10));
        //LabelsCoordinatePlayer2.append(CerentLabel);
        ui->glClientMap->addWidget(CerentLabel,i+1,0,1,1);
    }

    for(int i = 1; i <= 10; i++)
        for(int j = 1; j <= 10; j++ )
        {
            BaseField * CerentLabel = LabelsFieldPlayer2.at(CurentBaseField);
            CerentLabel->SetCoordinate(j,i);
            connect(CerentLabel, SIGNAL(clickedField(int, int)), this, SLOT(shot(int, int)));
            //LabelsFieldPlayer2.append(CerentLabel);

            ui->glClientMap->addWidget(CerentLabel,i,j,1,1);
            CurentBaseField ++;
        }
}

bool game_map::CheckShot(int px, int py)//check the field where the second player shot
{
    bool returnDeck = false;
    BaseField * Temp = SearchField(LabelsField, px,py);
    Temp->SerDamaged(true);
    if(Temp->GetDeck())
    {
        returnDeck = true;
        if(CheckKilled(Temp))
        {
            if(CheckAllShipsKilled())
            {
                writeMessage(CreateMessageGameOver(px,py));
                ui->lStatus->setText("Game Over \n\n Winner: " + Player2Name);
                GameOver = true;
            }else
            {
                writeMessage(CreateMessageKilled(px,py));
            }
        }else
        {
            writeMessage(CreateMessageHit(px,py));
        }
    }else
    {
        returnDeck = false;
        writeMessage(CreateMessageMiss(px,py));
    }


    Temp->RefreshPixmap();
    return returnDeck;
}

bool game_map::CheckShot(QByteArray px, QByteArray py)//check the field where the second player shot
{
    int x, y;
    bool ok;
    x = px.toInt(&ok, 10);
    y = py.toInt(&ok, 10);
    return CheckShot(x, y);
}

bool game_map::CheckDamagedField(int px, int py)//check whether the shot was fired in the field
{
    bool returnDamaged = true;
    BaseField * Temp = SearchField(LabelsFieldPlayer2, px,py);
    if(Temp->GetDamaged())
    {
        returnDamaged = false;
    }

    return returnDamaged;
}

void game_map::miss(QByteArray px, QByteArray py)//filling the playing field if the user missed
{
    int x, y;
    bool ok;
    x = px.toInt(&ok, 10);
    y = py.toInt(&ok, 10);
 BaseField * Temp = SearchField(LabelsFieldPlayer2, x, y);
 EndOfTurn();
 Temp->SerDamaged(true);
 Temp->RefreshPixmap();
}

void game_map::hit(QByteArray px, QByteArray py)//filling the playing field if the user hits
{
    int x, y;
    bool ok;
    x = px.toInt(&ok, 10);
    y = py.toInt(&ok, 10);
    BaseField * Temp = SearchField(LabelsFieldPlayer2, x, y);
    deck* CurrentDeck = new deck();
    Temp->AddDeck(CurrentDeck);
    Temp->SerDamaged(true);
    Temp->RefreshPixmap();
    EndOfTurn();

}

bool game_map::CheckKilled(BaseField * Field)//check if the ship was killed when hit
{
    bool returnKilled = true;
    int IDShip = Field->GetIDShip();
    deck * Temp ;
    int sizeList = listDeck.size();
    for(int i = 0; i < sizeList; i++)
    {
        Temp = listDeck.at(i);
        if(!Temp->Getdamaged() && Temp->GetShipID() ==IDShip )
        {
            returnKilled = false;
            break;
        }
    }
    return returnKilled;

}

void game_map::killed(QByteArray px, QByteArray py)//filling the playing field if the user killed the ship
{
    int x, y;
    bool ok;
    x = px.toInt(&ok, 10);
    y = py.toInt(&ok, 10);
    killed(x,y);
    EndOfTurn();

}

void game_map::killed(int px, int py)//filling the playing field if the user killed the ship
{
    BaseField * Temp=SearchField(LabelsFieldPlayer2, px, py);
    Temp->SerCheckKilled(true);
    if(!Temp->GetDeck())
    {
        deck* CurrentDeck = new deck();
        Temp->AddDeck(CurrentDeck);
        Temp->SerDamaged(true);
    }

        for(int i=px - 1; i <= px + 1; i++ )
            for(int j = py-1; j <= py + 1; j++)
            {
                if(i > 0 && i < 11 && j > 0 && j < 11 && (i != px || j != py))
                {   Temp = SearchField(LabelsFieldPlayer2, i, j);
                    Temp->SerDamaged(true);
                    if(Temp->GetDeck()&& !Temp->GetCheckKilled())
                    {
                        killed(i,j);
                    }
                    Temp->RefreshPixmap();
                }
            }


}

bool game_map::CheckAllShipsKilled()//check if all ships are killed
{
    bool returnAllkilled = true;
    deck * Temp ;
    int sizeList = listDeck.size();
    for(int i = 0; i < sizeList; i++)
    {
        Temp = listDeck.at(i);
        if(!Temp->Getdamaged() )
        {
            returnAllkilled = false;
            break;
        }
    }
    return returnAllkilled;

}

void game_map::RefreshSecondsLeft(QByteArray pSeconds)//display of the remaining seconds until the end of the move
{
    ui->lStatus->setText("Run walks: " +  Player2Name + "   " + pSeconds + " seconds");
}

void game_map::ChangePlayer()// player change message processing
{
    if(!GameOver)
    {
        BeginOfTurn();
    }
}

void game_map::BeginOfTurn()// start move user
{
    BeginOT();
    ActivePlayer = true;
    SecondsLeft = RunningTime;
    timerRunningTime->start();
}

void game_map::EndOfTurn()// end move user
{
    EndOT();
    ActivePlayer = false;
    timerRunningTime->stop();
    SecondsLeft = 0;
    writeMessage(CreateMessageChangePlayer());
}

void game_map::CreateServer()// server creation
{
    server = new gameserver();
    connect(server, SIGNAL(ServerStatus(QString)), this, SLOT(ServerStatus(QString)));
    connect(this, SIGNAL(startServer()), server, SLOT(startServer()));
    connect(server, SIGNAL(ClientConnect(QTcpSocket*)), this, SLOT(ClientConnect(QTcpSocket*)));
    startServer();
    ui->pbConnect->setVisible(false);
    //ActivePlayer = true;

}

void game_map::ServerStatus(QString Status)// server status
{
    ui->lStatus->setText(Status);
}

void game_map::Connect(QString paramIPserver)//client socket connection over IP server
{
    IPserver = paramIPserver;
    Client = new QTcpSocket(this);
    connect(Client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(Client, SIGNAL(connected()), this, SLOT(SocketConnection()));
    connect(Client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionErrors(QAbstractSocket::SocketError)), Qt::DirectConnection);
    Client->connectToHost(IPserver,SERVER_PORT);
    ui->pbConnect->setVisible(false);
}

void game_map::Reconnecting()// reconnecting to server
{
    Connect(IPserver);
}

void game_map::ClientConnect(QTcpSocket * socClient)// external client connection
{
    Client = socClient;
    connect(Client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    writeMessage(CreateMessageName(name.toUtf8()));
    writeMessage(CreateMessageName(name.toUtf8()));


    CreateGUIShipLayout();
    //BeginOfTurn();
}

void game_map::UserName(QString wpName)//username assignment
{
    name = wpName;
    ui->lUserName->setText(name);
    this->setWindowTitle(wpName + " map");
}

void game_map::readyRead()// read message from socket
{
    if(Client->waitForConnected(30))
    {
        Client->waitForReadyRead(30);
        //Client->r
        data = Client->readAll();
        ParserMessage(data);
    }
}

void game_map::updateTime()//timer update
{
    SecondsLeft--;
    if(SecondsLeft > 0)
    {
        writeMessage(CreateMessageSecondsLeft(SecondsLeft));
        ui->lStatus->setText("Run walks: " +  name + "   " + QString::number(SecondsLeft, 10) + " seconds");
    }else
    {
        EndOfTurn();
    }
}

void game_map::SocketConnection()//socket connected to server
{

    writeMessage(CreateMessageName(name.toUtf8()));
   // ActivePlayer = false;
    CreateGUIShipLayout();
}

void game_map::connectionErrors(QAbstractSocket::SocketError err)// error connecting to server
{
    if(err == QAbstractSocket::ConnectionRefusedError )
    {   delete Client;
        QTimer::singleShot(2000, this, SLOT(Reconnecting()));
    }
}

 void game_map::ClickableLabel(int px, int py)
 {
     if(LabelsFieldClicked)
     {
         if(CheckCreateShip(px, py, CurentShip.size,CurentShip.Course))
         {
             CreateShip(px, py, CurentShip.size,CurentShip.Course);
             ui->lStatus->setText("ship set");
             if(CheckFinishCreateShip())
             {
                 LabelsFieldClicked = false;
                 DisconnectRadioButton();
                 DeletionGUIShipLayout();
                 CreateGUIStart();
             }
         }else
         {
             ui->lStatus->setText("you cannot put a ship in this position");
         }
     }

 }

 void game_map::rbShip1_clicked()
 {
     CurentShip.amountShip = &amountShip1;
     CurentShip.rbShip = rbShip1;
     CurentShip.lShip = lShip1;
     CurentShip.size = 1;
 }
 void game_map::rbShip2_clicked()
 {
     CurentShip.amountShip = &amountShip2;
     CurentShip.rbShip = rbShip2;
     CurentShip.lShip = lShip2;
     CurentShip.size = 2;
 }
 void game_map::rbShip3_clicked()
 {
     CurentShip.amountShip = &amountShip3;
     CurentShip.rbShip = rbShip3;
     CurentShip.lShip = lShip3;
     CurentShip.size = 3;
 }
 void game_map::rbShip4_clicked()
 {
     CurentShip.amountShip = &amountShip4;
     CurentShip.rbShip = rbShip4;
     CurentShip.lShip = lShip4;
     CurentShip.size = 4;
 }

 void game_map::rbUP_clicked()
 {
     CurentShip.rbCourse = rbUP;
     CurentShip.Course = UP;
 }
 void game_map::rbDOWN_clicked()
 {
     CurentShip.rbCourse = rbDOWN;
     CurentShip.Course = DOWN;
 }
 void game_map::rbLEFT_clicked()
 {
     CurentShip.rbCourse = rbLEFT;
     CurentShip.Course = LEFT;
 }
 void game_map::rbRIGHT_clicked()
 {
     CurentShip.rbCourse = rbRIGHT;
     CurentShip.Course = RIGHT;
 }

 void game_map::StartGame_clicked()//puts the user in a ready state
 {
     disconnect(StartGame, SIGNAL(clicked()), this, SLOT(StartGame_clicked()));
     delete StartGame;
     //ui->glClientMap->removeWidget(StartGame);

     PreparedUser = true;
     CreateMapPlayer2();
     if (CheckPrepared())
     {

         EndOfTurn();
     }else
     {
         writeMessage(CreateMessagePrepared());
     }
 }

 void game_map::shot(int px, int py)// shot on the field of the second player
 {

     if (ActivePlayer && CheckDamagedField(px, py))
     {

         writeMessage(CreateMessageShot(px, py));
     }
 }

void game_map::on_pbConnect_clicked()
{
    startdlg = new connect_dialog();
    connect(startdlg, SIGNAL(CreateServer()), this, SLOT(CreateServer()));
    connect(startdlg, SIGNAL(Connect(QString)), this, SLOT(Connect(QString)));
    connect(startdlg, SIGNAL(UserName(QString)), this, SLOT(UserName(QString)));
    startdlg->show();
}
