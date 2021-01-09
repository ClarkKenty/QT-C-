#ifndef APPLICATION_H
#define APPLICATION_H

#include <QMainWindow>

namespace Ui {
class Application;
}

class Application : public QMainWindow
{
    Q_OBJECT

public:
    explicit Application(QWidget *parent = nullptr);
    ~Application();
    int mode;

private slots:
    void on_pushButton_clicked();

    void on_billing_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Application *ui;
};

#endif // APPLICATION_H
