#include "basefield.h"

BaseField::BaseField()
{
    Deck = NULL;
    pixField = QPixmap(":/images/sea.png");
    pixFieldDamaged = QPixmap(":/images/seaCheck.png");
    RefreshPixmap();
}

void BaseField::SetCoordinate(int pX, int pY)
{
    x = pX;
    y = pY;
}


void BaseField::GetCoordinate(int* pX, int* pY)
{
    *pX = x;
    *pY = y;
}
void BaseField::mousePressEvent(QMouseEvent* event)
{
    emit clickedField(x, y);
}


void BaseField::AddDeck( deck* pDeck)
{
    Deck = pDeck;
    Deck->SetCoordinate(x, y);
    RefreshPixmap();
}

void BaseField::SerDamaged(bool pdamag)
{
    damaged = pdamag;
    if(Deck !=NULL)
    {
        Deck->Setdamaged(damaged);
    }
    RefreshPixmap();
}

void BaseField::RefreshPixmap()
{
    if(Deck !=NULL)
    {
        this->setPixmap(Deck->GetPixmap());
    }else
    {
        if(damaged)
        {
            this->setPixmap(pixFieldDamaged);
        }else
        {
            this->setPixmap(pixField);
        }
    }
}

bool BaseField::GetDeck()
{
    if(Deck ==NULL)
    {
        return false;
    }
    return true;
}
bool BaseField::GetDamaged()
{
    return damaged;
}


int BaseField::GetIDShip()
{
    if(Deck !=NULL)
    {
       return Deck->GetShipID();
    }
    return -1;
}


void BaseField::SerCheckKilled(bool Check)
{
    CheckKilled= Check;
}
bool BaseField::GetCheckKilled()
{
    return CheckKilled;
}


