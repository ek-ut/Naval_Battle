#include "connect_dialog.h"
#include "ui_connect_dialog.h"

connect_dialog::connect_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connect_dialog)
{
    ui->setupUi(this);
    ui->lEName->setText("User");
    ui->le_IP->setText("127.0.0.1");
}

connect_dialog::~connect_dialog()
{
    delete ui;
}

void connect_dialog::on_pb_Connect_clicked()
{
    UserName(ui->lEName->text());
    QString ipConnect;
    ipConnect = ui->le_IP->text();
    Connect(ipConnect);
    this->close();
}

void connect_dialog::on_pb_create_clicked()
{
    CreateServer();
    UserName(ui->lEName->text());
    this->close();
}
