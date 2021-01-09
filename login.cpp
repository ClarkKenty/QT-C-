#include "login.h"
#include "ui_login.h"
#include "application.h"
#include <QMessageBox>

extern int suc;

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    user = 0;
}

Login::~Login()
{
    delete ui;
}


void Login::on_Login_Button_clicked()
{
    QString account = ui->Account->text();
    QString password = ui->Password->text();
    if(account=="" || password=="")
    {
        QMessageBox* errori = new QMessageBox();
        errori->setWindowTitle("输入为空");
        errori->setText("请输入账号和密码");
        errori->show();
        return;
    }
    if (account == "10000" && password == "12312")
    {user = 1;}
    else if (account == "10001" && password == "11111"){
        user = 2;
    }
    else{
        QMessageBox* error = new QMessageBox();

        error->setWindowTitle("请重新输入");
        error->setText("账号或密码错误");
        error->show();
        return;
    }
    Application* app = new Application();
    if(user == 1)
    {
        app->setWindowTitle("点菜管理系统(管理员）");
        app->mode = 1;
    }
    else if(user == 2)
    {
        app->setWindowTitle("点菜管理系统");
        app->mode = 2;
    }

    close();
    app->show();

}
