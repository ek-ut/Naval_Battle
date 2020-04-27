#ifndef BASEFIELD_H
#define BASEFIELD_H

#include <QObject>
#include "clickablelabel.h"
#include "deck.h"

class BaseField : public ClickableLabel
{
public:
    BaseField();
    void SetCoordinate(int pX, int pY);
    void GetCoordinate(int* pX, int* pY);
    void AddDeck( deck* pDeck);
    void SerDamaged(bool pdamag);
    void SerCheckKilled(bool pdamag);
    bool GetCheckKilled();
    bool GetDeck();
    bool GetDamaged();
    void RefreshPixmap();
    int GetIDShip();

signals:
    //void clickedField(int px, int py);//widget click event

protected:
     void mousePressEvent(QMouseEvent* event);//mouse event handler

private:
    int x;
    int y;
    deck* Deck;
    bool damaged = false;
    QPixmap pixField;
    QPixmap pixFieldDamaged;
    bool CheckKilled = false;
};

#endif // BASEFIELD_H
