#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "controller.hpp"

namespace Ui {
class MainWindow;
}

extern Controller* cur_user;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void init_set();
    void login();
    void user_add();
    void grant_add();
    void client_add();
    void staff_add();
    void medicine_add();
    void sales_add();
    void warehouse_add();
    void supplier_add();
    void data_print();
    ~MainWindow();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_query_clicked();

    void on_del_clicked();

    void on_add_clicked();

    void on_submit_clicked();


    void on_add_ok_clicked();

    void on_select_clicked();

private:
    Ui::MainWindow *ui;
    QTableWidget *current;
    QStringList head;
    QWidgetList head_index;
    bool is_changed;
    bool have_login;
};

#endif // MAINWINDOW_H
