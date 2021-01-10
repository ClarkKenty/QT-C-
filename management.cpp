#include "management.h"
#include "ui_management.h"
#include <QFile>
#include <QTextStream>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QValidator>
#include <QMessageBox>
#include <QDebug>
management::management(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::management)
{
    ui->setupUi(this);
    QFile file1("count.txt");
    QFile file2("history.txt");
    QFile file3("saves.txt");
    file1.open(QIODevice::ReadOnly| QIODevice::Text);
    QTextStream filecount(&file1);
    filecount.setCodec("utf-8");
    file2.open(QIODevice::ReadOnly| QIODevice::Text);
    QTextStream filehist(&file2);
    filehist.setCodec("utf-8");
    file3.open(QIODevice::ReadOnly| QIODevice::Text);
    QTextStream filesave(&file3);
    filesave.setCodec("utf-8");
    QString allhis = filehist.readAll();
    ui->historyorder->append(allhis);
    QString allsaves = filesave.readAll();
    ui->order_sum->setText(allsaves);
    file1.close();
    file2.close();
    file3.close();
}

management::~management()
{
    delete ui;
}

void management::on_pushButton_2_clicked()
{
    QFile file("history.txt");
    file.open(QIODevice::WriteOnly);
    file.close();
    ui->historyorder->clear();
}

void management::on_pushButton_clicked()
{
    QFile file1("count.txt");
    QFile file3("saves.txt");
    file1.open(QIODevice::WriteOnly| QIODevice::Text);
    file3.open(QIODevice::WriteOnly| QIODevice::Text);
    file1.close();
    file3.close();
    ui->order_sum->clear();
}

void management::on_pushButton_3_clicked()
{
    QFile file1("count.txt");
    QFile file3("saves.txt");
    file1.open(QIODevice::ReadWrite| QIODevice::Text);
    file3.open(QIODevice::ReadOnly| QIODevice::Text);
    QTextStream file1t(&file1);
    int nums;
    file1t>>nums;
    nums-=1;
    //
    QDialog diag;
    diag.setWindowTitle("修改");
    QFormLayout form(&diag);
    form.setVerticalSpacing(15);
    form.addRow(new QLabel("输入修改订单的编号："));
    QLineEdit* num1 = new QLineEdit(&diag);
    form.addRow("编号：",num1);
    form.addRow(new QLabel("输入需要修改的信息"));
    QLineEdit* money = new QLineEdit(&diag);
    form.addRow("订单金额",money);
    QLineEdit* time = new QLineEdit(&diag);
    form.addRow("时间",time);
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
        if(num1->text().toInt()>nums || num1->text().toInt()<=0)
        {
            QMessageBox* warning = new QMessageBox();
            warning->setWindowTitle("Warning");
            warning->setText("该项不存在");
            warning->show();
            return;
        }
        int serial = num1->text().toUInt();
        QString moneys = money->text();
        QString times = time->text();
        QTextStream fileedit(&file3);
        fileedit.setCodec("utf-8");
        QString tempsaves;
        int count = 1;
        while(!fileedit.atEnd())
        {
            if(count!=serial)
            {
                tempsaves+=fileedit.readLine();
                count++;
            }
            else{
                QString edit = fileedit.readLine();
                if(moneys!="")
                {
                    int index1 = edit.indexOf("额");
                    int index2 = edit.indexOf(" 时");
                    edit.replace(index1+2,index2-index1-2,moneys);
                }
                if(times!="")
                {
                    int index1 = edit.indexOf("间：");
                    int index2 = edit.size()-1;
                    edit.replace(index1+2,index2-index1-2,times);
                }
                tempsaves+=edit;
                count++;
            }
            tempsaves+="\n";
        }
        file1.close();
        file3.close();
        file3.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream editor(&file3);
        editor.setCodec("utf-8");
        editor<<tempsaves;
        ui->order_sum->setText(tempsaves);
    }
}
