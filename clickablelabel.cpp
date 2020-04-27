#include "clickablelabel.h"

ClickableLabel::ClickableLabel(const QString& text, QWidget* parent)
    : QLabel(parent)
{
    setText(text);
}

ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    QPoint p = QCursor::pos(); //gets the global cursor position
    mousePoint = mapFromGlobal(p); //get cursor position relative to this widget
    emit clicked();
}


QPoint ClickableLabel::GetMousePoint()
{
    return mousePoint;
}

