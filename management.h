#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include <QWidget>

namespace Ui {
class management;
}

class management : public QWidget
{
    Q_OBJECT

public:
    explicit management(QWidget *parent = nullptr);
    ~management();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::management *ui;
};

#endif // MANAGEMENT_H
