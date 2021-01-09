#include "application.h"
#include "ui_application.h"
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QString>
#include <QtDebug>
#include <QMessageBox>

QVector<QString> names;
QVector<QString> category;
QVector<int> price;
QVector<int> num;
QVector<int> saves;

Application::Application(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Application)
{
    ui->setupUi(this);
    mode = 0;
}

Application::~Application()
{
    delete ui;
}

void Application::on_pushButton_clicked()
{
    QDialog diag;
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
    if(diag.exec()==QDialog::Accepted)
    {
        if(food_name->text()=="" || food_class->text()=="" || food_price->text()=="" || food_num->text()=="")
        {
            QMessageBox* warning = new QMessageBox();
            warning->setWindowTitle("Warning");
            warning->setText("请输入其余点餐信息");
            warning->show();
            return;
        }
        names.push_back(food_name->text());
        category.push_back(food_class->text());
        price.push_back(food_price->text().toInt());
        num.push_back(food_num->text().toInt());
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
        diag.close();
    }
}

void Application::on_billing_clicked()
{
    ui->table->clear();
    ui->table->setRowCount(0);
    int totalprice = 0;
    for(int i =0;i<num.size();i++)
    {
        totalprice+=num[i]*price[i];
    }
    saves.push_back(totalprice);
    names.clear();
    category.clear();
    names.clear();
    num.clear();
    QMessageBox* bill = new QMessageBox();
    bill->setWindowTitle("结账金额");
    bill->setText("总金额为：" + QString::number(totalprice));
    bill->show();
}

void Application::on_pushButton_3_clicked()
{
    QDialog diag;
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
        int del = num1->text().toInt();
        int currows = ui->table->rowCount();
        if(currows >= del)
        {
            ui->table->removeRow(del-1);
            names.erase(names.begin()+del-1);
            num.erase(num.begin()+del-1);
            price.erase(price.begin()+del-1);
            category.erase(category.begin()+del-1);
        }

        else{
            QMessageBox* mes = new QMessageBox();
            mes->setWindowTitle("Warning");
            mes->setText("该项不存在！");
            mes->show();
        }
    }
}

void Application::on_pushButton_2_clicked()
{
    QDialog diag;
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
    if(diag.exec()==QDialog::Accepted)
    {
        if(num1->text()=="")
        {
            QMessageBox* warning = new QMessageBox();
            warning->setWindowTitle("Warning");
            warning->setText("请输入修改项序号");
            warning->show();
            return;
        }
        if(num1->text().toInt()>ui->table->rowCount())
        {
            QMessageBox* warning = new QMessageBox();
            warning->setWindowTitle("Warning");
            warning->setText("该项不存在");
            warning->show();
            return;
        }
        int serial = num1->text().toUInt();
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
