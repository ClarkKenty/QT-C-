#include "application.h"
#include "ui_application.h"
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QString>
#include <QtDebug>
#include <QMessageBox>
#include <QDateTime>
#include "management.h"
#include <QFile>
#include "foodmenu.h"
#include <login.h>

QVector<QString> names;//菜品名字
QVector<QString> category;//菜品种类
QVector<int> price;//菜品价格
QVector<int> num;//菜品数量
int ordercount;//订单编号

Application::Application(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Application)
{
    ui->setupUi(this);
    mode = 0;//用户权限
}

Application::~Application()
{
    delete ui;
}

void Application::on_pushButton_clicked()//用户点击添加菜品按钮
{
    QDialog diag;//添加菜品对话框
    diag.setWindowTitle("添加菜品");
    diag.resize(200,20);
    QFormLayout form(&diag);
    form.setVerticalSpacing(15);
    form.addRow(new QLabel("输入新的菜品："));
    QLineEdit* food_name = new QLineEdit(&diag);
    form.addRow("名称",food_name);
    QLineEdit* food_class = new QLineEdit(&diag);
    form.addRow("种类",food_class);
    QLineEdit* food_price = new QLineEdit(&diag);
    food_price->setValidator(new QIntValidator(1,9999));
    form.addRow("单价(1-9999)",food_price);
    QLineEdit* food_num = new QLineEdit(&diag);
    food_num->setValidator(new QIntValidator(1,99));
    form.addRow("数量(1-99)",food_num);
    QDialogButtonBox button(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &diag);
    connect(&button,SIGNAL(accepted()),&diag,SLOT(accept()));
    connect(&button,SIGNAL(rejected()),&diag,SLOT(reject()));
    form.addRow(&button);
    if(diag.exec()==QDialog::Accepted)//若用户点击OK按钮
    {
        if(food_name->text()=="" || food_class->text()=="" || food_price->text()=="" || food_num->text()=="")//判断用户是否遗漏点单信息
        {
            QMessageBox* warning = new QMessageBox();
            warning->setWindowTitle("Warning");
            warning->setText("请输入所有点餐信息");
            warning->show();
            return;
        }
        //将输入的点单信息加入数组存储中
        names.push_back(food_name->text());
        category.push_back(food_class->text());
        price.push_back(food_price->text().toInt());
        num.push_back(food_num->text().toInt());
        int row = names.size()-1;//增加表格行数
        ui->table->insertRow(row);
        QTableWidgetItem* item1 = new QTableWidgetItem(*(names.end()-1));
        ui->table->setItem(row,0,item1);
        QTableWidgetItem* item2 = new QTableWidgetItem(*(category.end()-1));
        ui->table->setItem(row,1,item2);
        QTableWidgetItem* item3 = new QTableWidgetItem(QString::number(*(price.end()-1)));
        ui->table->setItem(row,2,item3);
        QTableWidgetItem* item4 = new QTableWidgetItem(QString::number(*(num.end()-1)));
        ui->table->setItem(row,3,item4);
        int total = price.back()*num.back();
        QTableWidgetItem* item5 = new QTableWidgetItem(QString::number(total));
        ui->table->setItem(row,4,item5);
        diag.close();
    }
}

void Application::on_billing_clicked()//用户点击结账按钮
{
    int totalprice = 0;//生成总金额
    for(int i =0;i<num.size();i++)
    {
        totalprice+=num[i]*price[i];
    }
    if(totalprice == 0)//用户添加菜品信息
    {
        QMessageBox* warning = new QMessageBox();
        warning->setWindowTitle("Warning");
        warning->setText("无法生成空白订单!");
        warning->show();
        return;
    }
    QDialog diag;
    diag.setWindowTitle("服务员");
    QFormLayout form(&diag);
    form.setVerticalSpacing(15);
    form.addRow(new QLabel("输入服务员编号："));
    QLineEdit* waiter = new QLineEdit(&diag);
    form.addRow("编号：",waiter);
    QDialogButtonBox button(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &diag);
    form.addRow(&button);
    connect(&button,SIGNAL(accepted()),&diag,SLOT(accept()));
    connect(&button,SIGNAL(rejected()),&diag,SLOT(reject()));
    QString waiterid;
    if(diag.exec()==QDialog::Accepted)//若用户点击OK按钮
    {
        waiterid = waiter->text();
        qDebug()<<"a";
        diag.close();
    }
    else{
        QMessageBox* fail = new QMessageBox();
        fail->setWindowTitle("warning");
        fail->setText("请输入服务员编号！");;
        fail->show();
        diag.close();
        return;
    }
    QDateTime current_time = QDateTime::currentDateTime();//获取当前时间
    QString date_time = current_time.toString("yyyy.MM.dd hh:mm:ss");
    ui->table->clearContents();//结账完成，清空当前点单表格
    ui->table->setRowCount(0);
    QMessageBox* bill = new QMessageBox();//显示订单总金额提示框
    bill->setWindowTitle("订单金额");
    bill->setText("创建订单成功！总金额为：" + QString::number(totalprice));
    bill->show();
    ui->suclog->append("生成订单! 订单金额："+QString::number(totalprice)+" 时间：" + date_time+" 服务员："+waiterid);
    ui->suclog->append("======订单内容======");
    QFile file2("count.txt");//该文件记录总订单数
    file2.open(QIODevice::ReadWrite| QIODevice::Text);//先读取当前订单数
    QTextStream getnum(&file2);
    QString numcount = getnum.readLine();
    if(numcount!="")
    {
        ordercount = numcount.toInt();
    }
    else ordercount = 1;
    file2.close();//读取完毕，关闭文件
    file2.open(QIODevice::WriteOnly| QIODevice::Text);//在当前订单数的基础上加1，并重新写入文件
    QTextStream writenum(&file2);
    writenum<<ordercount+1;
    file2.close();//写入完毕，关闭文件
    QString logg;//记录订单日志
    QString logg2;//记录订单汇总
    logg2.append("订单编号：#"+QString::number(ordercount) + " 订单金额："+QString::number(totalprice)+" 时间：" + date_time+" 服务员："+waiterid+"\n");
    logg.append("订单金额："+QString::number(totalprice)+" 时间：" + date_time+" 服务员："+waiterid+"\n");
    QFile filehis("saves.txt");//将订单汇总写入文件
    filehis.open(QIODevice::WriteOnly| QIODevice::Text | QIODevice::Append);
    QTextStream writee(&filehis);
    writee.setCodec("utf-8");
    writee<<logg2;
    filehis.close();
    logg.append("======订单内容======\n");
    for(int i =0 ;i<names.size();i++)
    {
        ui->suclog->append(names[i]+" x"+QString::number(num[i]));
        logg.append(names[i]+" x"+QString::number(num[i]));
        logg.append("\n");
    }

    names.clear();
    category.clear();
    names.clear();
    num.clear();
    ui->suclog->append("===================");
    logg.append("===================\n\n");
    QFile file("history.txt");//将订单日志写入文件
    file.open(QIODevice::WriteOnly| QIODevice::Text | QIODevice::Append);
    QTextStream writehis(&file);
    writehis.setCodec("utf-8");
    writehis<<logg;
    file.close();
}

void Application::on_pushButton_3_clicked()//用户点击删除按钮
{
    QDialog diag;//弹出删除信息对话框
    diag.setWindowTitle("删除");
    QFormLayout form(&diag);
    form.setVerticalSpacing(15);
    form.addRow(new QLabel("输入删除项的序号："));
    QLineEdit* num1 = new QLineEdit(&diag);
    form.addRow("序号：",num1);
    QDialogButtonBox button(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &diag);
    connect(&button,SIGNAL(accepted()),&diag,SLOT(accept()));
    connect(&button,SIGNAL(rejected()),&diag,SLOT(reject()));
    form.addRow(&button);
    if(diag.exec()==QDialog::Accepted)
    {
        int del = num1->text().toInt();//在表格中删除用户指定行数
        int currows = ui->table->rowCount();
        if(currows >= del && del>0)//在存储信息的数组中删除用户指定行数
        {
            ui->table->removeRow(del-1);
            names.erase(names.begin()+del-1);
            num.erase(num.begin()+del-1);
            price.erase(price.begin()+del-1);
            category.erase(category.begin()+del-1);
        }

        else{//用户输入的编号超出数据范围，弹出提示
            QMessageBox* mes = new QMessageBox();
            mes->setWindowTitle("Warning");
            mes->setText("该项不存在！");
            mes->show();
        }
    }
}

void Application::on_pushButton_2_clicked()//用户点击修改按钮
{
    QDialog diag;//弹出修改信息对话框
    diag.setWindowTitle("修改");
    QFormLayout form(&diag);
    form.setVerticalSpacing(15);
    form.addRow(new QLabel("输入修改项的序号："));
    QLineEdit* num1 = new QLineEdit(&diag);
    form.addRow("序号：",num1);
    form.addRow(new QLabel("输入需要修改的信息"));
    QLineEdit* food_name = new QLineEdit(&diag);
    form.addRow("名称",food_name);
    QLineEdit* food_class = new QLineEdit(&diag);
    form.addRow("种类",food_class);
    QLineEdit* food_price = new QLineEdit(&diag);
    food_price->setValidator(new QIntValidator(1,9999));
    form.addRow("单价(1-9999)",food_price);
    QLineEdit* food_num = new QLineEdit(&diag);
    food_num->setValidator(new QIntValidator(1,99));
    form.addRow("数量(1-99)",food_num);
    QDialogButtonBox button(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &diag);
    connect(&button,SIGNAL(accepted()),&diag,SLOT(accept()));
    connect(&button,SIGNAL(rejected()),&diag,SLOT(reject()));
    form.addRow(&button);
    if(diag.exec()==QDialog::Accepted)//若用户点击OK按钮
    {
        if(num1->text()=="")//用户未输入修改项序号，弹出提示
        {
            QMessageBox* warning = new QMessageBox();
            warning->setWindowTitle("Warning");
            warning->setText("请输入修改项序号");
            warning->show();
            return;
        }
        if(num1->text().toInt()>ui->table->rowCount() || num1->text().toInt()<=0)//用户输入的序号超出范围，弹出提示信息
        {
            QMessageBox* warning = new QMessageBox();
            warning->setWindowTitle("Warning");
            warning->setText("该项不存在");
            warning->show();
            return;
        }
        int serial = num1->text().toUInt();//将输入的字符串转为整型序号
        //获取用户输入的各部分信息并进行修改
        QString s1 = food_name->text();
        if(s1!="")
        {
            QTableWidgetItem* item1 = new QTableWidgetItem(s1);
            names[serial-1] = s1;
            ui->table->setItem(serial-1,0,item1);
        }
        QString s2 = food_class->text();
        if(s2!="")
        {
            category[serial-1] = s2;
            QTableWidgetItem* item2 = new QTableWidgetItem(s2);
            ui->table->setItem(serial-1,1,item2);
        }
        QString s3 = food_price->text();
        if(s3!="")
        {
            price[serial-1] = s3.toInt();
            QTableWidgetItem* item3 = new QTableWidgetItem(s3);
            ui->table->setItem(serial-1,2,item3);
        }
        QString s4 = food_num->text();
        if(s4!="")
        {
            num[serial-1] = s4.toInt();
            QTableWidgetItem* item4 = new QTableWidgetItem(s4);
            ui->table->setItem(serial-1,3,item4);
        }
        int t = num[serial-1]*price[serial-1];
        QString s5 = QString::number(t);
        QTableWidgetItem* item5 = new QTableWidgetItem(s5);
        ui->table->setItem(serial-1,4,item5);
        diag.close();
    }
}

void Application::on_billing_2_clicked()//用户点击管理按钮
{
    if(mode==2)//用户登录的账号为普通权限账号，提示权限不足
    {
        QMessageBox* fail = new QMessageBox();
        fail->setWindowTitle("失败");
        fail->setText("操作失败！需要管理员权限！");;
        fail->show();
        return;
    }
    //用户登录的账号为管理员账号，进入下一步操作
    management* manage = new management();
    manage->setWindowTitle("订单管理");
    manage->show();
}

void Application::on_pushButton_4_clicked()//用户点击菜单按钮
{
    foodmenu* menuwidget = new foodmenu();
    menuwidget->setWindowTitle("菜单");
    menuwidget->show();//显示菜单界面
    connect(menuwidget,SIGNAL(GetUdpLogMsg(QVector<QString>)),this,SLOT(changetable(QVector<QString>)));//将菜单部件传回的值与当前程序建立链接
}

void Application::changetable(QVector<QString> msg)//从菜单传回的值存储到当前数组，并更改点餐表格
{
    names.push_back(msg[0]);
    category.push_back(msg[1]);
    price.push_back(msg[2].toUInt());
    num.push_back(msg[3].toUInt());
    int row = names.size()-1;
    ui->table->insertRow(row);
    QTableWidgetItem* item1 = new QTableWidgetItem(*(names.end()-1));
    ui->table->setItem(row,0,item1);
    QTableWidgetItem* item2 = new QTableWidgetItem(*(category.end()-1));
    ui->table->setItem(row,1,item2);
    QTableWidgetItem* item3 = new QTableWidgetItem(QString::number(*(price.end()-1)));
    ui->table->setItem(row,2,item3);
    QTableWidgetItem* item4 = new QTableWidgetItem(QString::number(*(num.end()-1)));
    ui->table->setItem(row,3,item4);
    int total = price.back()*num.back();
    QTableWidgetItem* item5 = new QTableWidgetItem(QString::number(total));
    ui->table->setItem(row,4,item5);
}

void Application::on_billing_3_clicked()//用户点击退出按钮
{
    Login* lo = new Login();
    lo->show();
    close();
}
