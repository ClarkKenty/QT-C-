#ifndef FOODMENU_H
#define FOODMENU_H

#include <QWidget>

namespace Ui {
class foodmenu;
}

class foodmenu : public QWidget
{
    Q_OBJECT

public:
    explicit foodmenu(QWidget *parent = nullptr);
    ~foodmenu();
signals:
    void GetUdpLogMsg(QVector<QString> msg);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::foodmenu *ui;
};

#endif // FOODMENU_H
/*信号传输操作：
 * 1.在父窗口的头文件加入接受函数
 private slots:
        void changetable(QString msg);
    2.在子窗口头文件加入信号函数
    signals:
     void GetUdpLogMsg(QString msg);
     3.在父窗口的定义中进行函数关联
     connect(menuwidget,SIGNAL(GetUdpLogMsg(QString)),this,SLOT(changetable(QString)));
     4.定义changetable(QString msg);
     5.在子窗口发送位置加入emit GetUdpLogMsg(str);（发送str给父窗口）（父窗口将调用GetUdpLogMsg(str)）
    */
