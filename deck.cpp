#include "deck.h"

deck::deck()
{
    damaged = false;
    pixDeck = QPixmap(":/images/deck.png");
    pixDeckDamaged = QPixmap(":/images/DeckDamaged.png");
}

void deck::SetCoordinate(int pX, int pY)
{
    X = pX;
    Y = pY;
}
void deck::GetCoordinate(int* pX, int* pY)
{
    *pX = X;
    *pY = Y;
}
void deck::SetShipID(int pShipID)
{
    ShipID = pShipID;
}
int deck::GetShipID()
{
    return ShipID;
}
void deck::Setdamaged(bool pDamaged)
{
    damaged = pDamaged;
}
bool deck::Getdamaged()
{
    return damaged;
}

QPixmap deck::GetPixmap()
{
    if(damaged)
    {
        return pixDeckDamaged;
    }
    return pixDeck;
}
