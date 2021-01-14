#include "foodmenu.h"
#include "ui_foodmenu.h"
#include <QMessageBox>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QFile>
#include <QTextStream>
#include <QStringList>

foodmenu::foodmenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::foodmenu)
{
    ui->setupUi(this);
    ui->countedit->setValidator(new QIntValidator(1,99));//限制用户的数量输入在(1-99)范围内
    QFile file1("rate.txt");//获取之前存储的的评分数据
    QVector<int> ratings;
    file1.open(QIODevice::ReadOnly| QIODevice::Text);
    QTextStream filerate(&file1);
    while(!filerate.atEnd())
    {
        QString info = filerate.readLine();
        QStringList lists = info.split(" ");
        ratings.push_back(lists[0].toInt());
        ratings.push_back(lists[1].toInt());
    }
    for(int i = 0;i<ratings.size();i+=2)//将之前的评分数据插入菜单表格
    {
        QTableWidgetItem* item = new QTableWidgetItem(QString::number(ratings[i+1]));
        ui->menulist->setItem(ratings[i]-1,3,item);
    }
    file1.close();
}

foodmenu::~foodmenu()
{
    delete ui;
}

void foodmenu::on_pushButton_clicked()//用户点击添加按钮
{
    if(ui->countedit->text()=="")//用户未输入数量信息，弹出提示框
    {
        QMessageBox* fail = new QMessageBox();
        fail->setWindowTitle("warning");
        fail->setText("请输入数量！");;
        fail->show();
        return;
    }
    QList<QTableWidgetItem*>items=ui->menulist->selectedItems();//获取当前用户选中的菜品
    if(items.size()==0)//用户未进行选择，弹出提示
    {
        QMessageBox* fail = new QMessageBox();
        fail->setWindowTitle("warning");
        fail->setText("请选择菜品！");;
        fail->show();
        return;
    }
    int row = ui->menulist->row(items[0]);//获取用户当前选中菜品的行数
    int count = ui->countedit->text().toUInt();//转为整数
    QVector<QString> mes ;//获取当前菜品的所有信息
    mes.push_back(ui->menulist->item(row,0)->text());
    mes.push_back(ui->menulist->item(row,1)->text());
    mes.push_back(ui->menulist->item(row,2)->text());
    mes.push_back(QString::number(count));
    emit GetUdpLogMsg(mes);//发射信号，将信息传回主应用界面
    this->close();
}

void foodmenu::on_pushButton_2_clicked()//用户点击评分按钮
{
    QDialog diag;//弹出评分输入对话框
    diag.setWindowTitle("评分");
    QFormLayout form(&diag);
    form.setVerticalSpacing(15);
    form.addRow(new QLabel("输入菜肴的序号："));
    QLineEdit* num1 = new QLineEdit(&diag);
    form.addRow("序号：",num1);
    form.addRow(new QLabel("输入评分："));
    QLineEdit* score = new QLineEdit(&diag);
    score->setValidator(new QIntValidator(1,9));
    form.addRow("评分(1-9):",score);
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
            warning->setText("请输入菜肴的序号!");
            warning->show();
            return;
        }
        if(num1->text().toInt()>39 || num1->text().toInt()<=0)
        {
            QMessageBox* warning = new QMessageBox();
            warning->setWindowTitle("Warning");
            warning->setText("该项不存在");
            warning->show();
            return;
        }
        int serial = num1->text().toUInt();
        QString rate = score->text();
        QTableWidgetItem* item5 = new QTableWidgetItem(rate);
        ui->menulist->setItem(serial-1,3,item5);
        QFile file1("rate.txt");//将评分记录进评分文件中
        file1.open(QIODevice::ReadWrite| QIODevice::Text |QIODevice::Append);
        QTextStream filerate(&file1);
        filerate<<serial<<" "<<rate<<"\n";
        file1.close();
        diag.close();
    }
}

void foodmenu::on_pushButton_3_clicked()//用户点击清除所有评分按钮
{
    QFile file1("rate.txt");
    file1.open(QIODevice::WriteOnly | QIODevice::Text);
    file1.close();
    for(int i = 0;i<39;i++)//清除当前表格所有的评分单元
    {
        ui->menulist->takeItem(i,3);
    }
}
