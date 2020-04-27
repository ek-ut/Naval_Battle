#ifndef DECK_H
#define DECK_H

#include <QObject>
#include <QPixmap>

class deck
{
public:
    deck();

    void SetCoordinate(int pX, int pY);
    void GetCoordinate(int* pX, int* pY);
    void SetShipID(int pShipID);
    int GetShipID();
    void Setdamaged(bool pDamaged);
    bool Getdamaged();
    QPixmap GetPixmap();
private:
    int X;
    int Y;
    int ShipID;
    bool damaged;
    QPixmap pixDeck;
    QPixmap pixDeckDamaged;
};

#endif // DECK_H
