#ifndef CONNECT_DIALOG_H
#define CONNECT_DIALOG_H

#include <QDialog>

namespace Ui {
class connect_dialog;
}

class connect_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit connect_dialog(QWidget *parent = nullptr);
    ~connect_dialog();

signals:
    void CreateServer();
    void UserName(QString name);
    void Connect(QString sIP);

private slots:
    void on_pb_Connect_clicked();

    void on_pb_create_clicked();

private:
    Ui::connect_dialog *ui;
};

#endif // CONNECT_DIALOG_H
