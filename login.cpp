#include "login.h"
#include "ui_login.h"
#include "application.h"
#include <QMessageBox>
#include<QFrame>
extern int suc;

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    setAttribute(Qt::WA_StyledBackground,true);
    ui->setupUi(this);
    user = 0;//默认为未登录状态
}

Login::~Login()
{
    delete ui;
}


void Login::on_Login_Button_clicked()//用户点击登录按钮
{
    QString account = ui->Account->text();//获取用户输入的账号
    QString password = ui->Password->text();//获取用户输入的密码
    if(account=="" || password=="")//输入为空，显示提示信息
    {
        QMessageBox* errori = new QMessageBox();
        errori->setWindowTitle("输入为空");
        errori->setText("请输入账号和密码");
        errori->show();
        return;
    }
    if (account == "10000" && password == "12312")//管理员账号
    {user = 1;}
    else if (account == "10001" && password == "11111"){//服务员账号
        user = 2;
    }
    else{//密码错误，显示提示框
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
    app->show();//密码正确，进入对应权限的主界面

}
