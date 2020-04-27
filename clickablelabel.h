/*
 * class ClickableLabel inherited from class QLabel
 * Added widget click event
 * Added receipt and storage of cursor position at the moment of click
 * Added method to get cursor position
 */
#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H
#include "QLabel"

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel( const QString& text="", QWidget* parent=NULL );
        ~ClickableLabel();
    QPoint GetMousePoint();//get cursor position
    signals:
        void clicked();//widget click event
        void clickedField(int px, int py);
    protected:
        virtual void mousePressEvent(QMouseEvent* event);//mouse event handler
private:
        QPoint mousePoint;
};

#endif // CLICKABLELABEL_H
