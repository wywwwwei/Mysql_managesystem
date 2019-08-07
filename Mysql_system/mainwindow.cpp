#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>

Controller* cur_user;

void MainWindow::init_set()
{
    ui->avatar->setPixmap(QPixmap("./1.PNG"));
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setTabEnabled(1,false);
    ui->user->horizontalHeader()->setVisible(true);
    ui->user->hide();
    ui->grant->horizontalHeader()->setVisible(true);
    ui->grant->hide();
    ui->sales->horizontalHeader()->setVisible(true);
    ui->sales->hide();
    ui->client->horizontalHeader()->setVisible(true);
    ui->client->hide();
    ui->staff->horizontalHeader()->setVisible(true);
    ui->staff->hide();
    ui->medicine->horizontalHeader()->setVisible(true);
    ui->medicine->hide();
    ui->supplier->horizontalHeader()->setVisible(true);
    ui->supplier->hide();
    ui->warehouse->horizontalHeader()->setVisible(true);
    ui->warehouse->hide();
    ui->add_ok->hide();
    ui->select->hide();
    ui->filter->hide();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Pharmaceutical Sales Management System");
    init_set();
    is_changed = false;
    have_login=false;
    cur_user = new Controller();
    cur_user->init();
}

void MainWindow::login()
{
    if(!have_login)
    {
    string username = ui->username->text().toStdString();
    string password = ui->password->text().toStdString();
    if(username.length()==0)
    {
        QMessageBox::warning(NULL, "WARNING","username cannot be null", QMessageBox::Yes, QMessageBox::Yes);
    }
    else if(password.length()==0){
        QMessageBox::warning(NULL, "WARNING","password connot be null" , QMessageBox::Yes, QMessageBox::Yes);
    }
    else {
        if(cur_user->login(username,password))
        {
            have_login = true;
            ui->username->setReadOnly(true);
            ui->password->setReadOnly(true);
            ui->submit->setText("LOGOUT");
            cur_user->getGrantUpdate();
            QStringList can_select;
            if(cur_user->curGrant("user")!=nullptr)
            {
                can_select<<"user";
                head_index.append(ui->user);
            }
            if(cur_user->curGrant("tablegrant")!=nullptr)
            {
                 can_select<<"tablegrant";
                 head_index.append(ui->grant);
            }
            if(cur_user->curGrant("medicine")!=nullptr)
            {
                can_select<<"medicine";
                head_index.append(ui->medicine);
            }
            if(cur_user->curGrant("client")!=nullptr)
            {
                can_select<<"client";
                head_index.append(ui->client);
            }
            if(cur_user->curGrant("supplier")!=nullptr)
            {
                can_select<<"supplier";
                head_index.append(ui->supplier);
            }
            if(cur_user->curGrant("staff")!=nullptr)
            {
                can_select<<"staff";
                head_index.append(ui->staff);
            }
            if(cur_user->curGrant("sales")!=nullptr)
            {
                can_select<<"sales";
                head_index.append(ui->sales);
            }
            if(cur_user->curGrant("warehouse")!=nullptr)
            {
                can_select<<"warehouse";
                head_index.append(ui->warehouse);
            }
            QObject::disconnect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(on_comboBox_currentIndexChanged(int)));
            ui->comboBox->addItems(can_select);
            QObject::connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(on_comboBox_currentIndexChanged(int)));
            if(!head_index.empty())
            {
                current = (QTableWidget*)head_index[0];
                current->show();
                bool*get_grant = cur_user->curGrant(ui->comboBox->currentText().toStdString());
                if(get_grant!=nullptr)
                {
                    if(get_grant[0])
                    {
                        ui->query->show();
                        ui->filter->show();
                        ui->select->show();
                    }
                    else
                    {
                        ui->query->hide();
                        ui->filter->hide();
                        ui->select->hide();
                    }
                    if(get_grant[1])
                        ui->add->show();
                    else
                        ui->add->hide();
                    if(get_grant[2])
                        ui->del->show();
                    else
                        ui->del->hide();
                }

            }
            ui->tabWidget->setTabEnabled(1,true);
            ui->tabWidget->setCurrentIndex(1);
        }
        else
        {
            QMessageBox::warning(NULL, "WARNING","Wrong username or password." , QMessageBox::Yes, QMessageBox::Yes);
        }
    }
    }
    else {
        cur_user->logout();
        current->hide();
        ui->username->setReadOnly(false);
        ui->password->setReadOnly(false);
        ui->submit->setText("LOGIN");
        QObject::disconnect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(on_comboBox_currentIndexChanged(int)));
        ui->comboBox->clear();
        QObject::connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(on_comboBox_currentIndexChanged(int)));
        head_index.clear();
        have_login = false;
        ui->tabWidget->setTabEnabled(1,false);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if(current!=nullptr)
    {
        current->hide();
    }
    current = (QTableWidget*)head_index[index];
    current->show();
    string cur_tablename;
    QString cur_table_name = ui->comboBox->currentText();
    bool*if_show = cur_user->curGrant(cur_table_name.toStdString());
    if(if_show!=nullptr)
    {
        if(if_show[0])
        {
            ui->query->show();
            ui->filter->show();
            ui->select->show();
        }
        else
        {
            ui->query->hide();
            ui->filter->hide();
            ui->select->hide();
        }
        if(if_show[1])
            ui->add->show();
        else
            ui->add->hide();
        if(if_show[2])
            ui->del->show();
        else
            ui->del->hide();
    }
}


void MainWindow::on_query_clicked()
{
    if(is_changed)
    {
        is_changed = false;
        ui->add_ok->hide();
    }
    QString cur_table_name = ui->comboBox->currentText();
    QTableWidget*cur_table=(QTableWidget*)head_index[ui->comboBox->currentIndex()];
    if(QString::compare(cur_table_name,"user")==0)
    {
        cur_user->select("user","*","");
    }
    else if(QString::compare(cur_table_name,"tablegrant")==0)
    {
        cur_user->select("tablegrant","*","");
    }
    else if(QString::compare(cur_table_name,"medicine")==0)
    {
        cur_user->select("medicine,supplier","medicine.mid,medicine.name,medicine.salesprice,medicine.purchaseprice,medicine.sid,supplier.sname","medicine.sid=supplier.sid");
    }
    else if(QString::compare(cur_table_name,"client")==0)
    {
        cur_user->select("client","*","");
    }
    else if(QString::compare(cur_table_name,"supplier")==0)
    {
        cur_user->select("supplier","*","");
    }
    else if(QString::compare(cur_table_name,"staff")==0)
    {
        cur_user->select("staff","*","");
    }
    else if(QString::compare(cur_table_name,"sales")==0)
    {
        cur_user->select("sales,medicine,staff,client","sales.medicineid,medicine.name,sales.staffid,staff.staffname,sales.clientid,client.name,sales.curdate","sales.medicineid=medicine.mid and sales.staffid=staff.staffid and sales.clientid=client.cid");
    }
    else if(QString::compare(cur_table_name,"warehouse")==0)
    {
        cur_user->select("warehouse,medicine","warehouse.waremid,medicine.name,warehouse.stocknum","warehouse.waremid=medicine.mid");
    }
    this->data_print();
}

void MainWindow::data_print()
{
    MYSQL_ROW row_data;
    MYSQL_RES *res = cur_user->get_result();
    int row_num = mysql_num_rows(res);
    int field_num = mysql_num_fields(res);
    current->setRowCount(row_num);
    for(int i = 0;i<row_num;i++)
    {
        row_data = mysql_fetch_row(res);
        for(int j = 0;j<field_num;j++)
        {
            current->setItem(i,j,new QTableWidgetItem(row_data[j]));
        }
    }
    current->setFocusPolicy(Qt::NoFocus);
}

void MainWindow::on_del_clicked()
{
    int row_index = current->currentRow();
    if(is_changed&&row_index==current->rowCount()-1)
    {
        is_changed = false;
        ui->add_ok->hide();
        current->removeRow(row_index);
        return;
    }
    if(row_index==-1)
        QMessageBox::warning(NULL, "WARNING", "You have to choose a line", QMessageBox::Yes, QMessageBox::Yes);
    else
    {
        QString cur_table_name = ui->comboBox->currentText();
        if(QString::compare(cur_table_name,"user")==0)
        {
            string filter = "username=\'"+current->item(row_index,0)->text().toStdString()+'\'';
            if(cur_user->delete_item("user",filter))
            {
                current->removeRow(row_index);
            }
            else
            {
                QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
            }
        }
        else if(QString::compare(cur_table_name,"tablegrant")==0)
        {
            string filter = "username=\'"+current->item(row_index,0)->text().toStdString()+"\' and tablename=\'"+current->item(row_index,1)->text().toStdString()+'\'';
            if(cur_user->delete_item("tablegrant",filter))
            {
                current->removeRow(row_index);
            }
            else
            {
                QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
            }
        }
        else if(QString::compare(cur_table_name,"medicine")==0)
        {
            string filter = "mid="+current->item(row_index,0)->text().toStdString();
            if(cur_user->delete_item("medicine",filter))
            {
                current->removeRow(row_index);
            }
            else
            {
                QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
            }
        }
        else if(QString::compare(cur_table_name,"client")==0)
        {
            string filter = "cid="+current->item(row_index,0)->text().toStdString();
            if(cur_user->delete_item("client",filter))
            {
                current->removeRow(row_index);
            }
            else
            {
                QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
            }
        }
        else if(QString::compare(cur_table_name,"supplier")==0)
        {
            string filter = "sid="+current->item(row_index,0)->text().toStdString();
            if(cur_user->delete_item("supplier",filter))
            {
                current->removeRow(row_index);
            }
            else
            {
                QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
            }
        }
        else if(QString::compare(cur_table_name,"staff")==0)
        {
            string filter = "staffid="+current->item(row_index,0)->text().toStdString();
            if(cur_user->delete_item("staff",filter))
            {
                current->removeRow(row_index);
            }
            else
            {
                QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
            }
        }
        else if(QString::compare(cur_table_name,"sales")==0)
        {
            string filter = "medicineid="+current->item(row_index,0)->text().toStdString()+" and staffid="+current->item(row_index,2)->text().toStdString()+" and clientid="+current->item(row_index,4)->text().toStdString();
            if(cur_user->delete_item("sales",filter))
            {
                current->removeRow(row_index);
            }
            else
            {
                QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
            }
        }
        else if(QString::compare(cur_table_name,"warehouse")==0)
        {
            string filter = "waremid="+current->item(row_index,0)->text().toStdString();
            if(cur_user->delete_item("warehouse",filter))
            {
                current->removeRow(row_index);
            }
            else
            {
                QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
            }
        }
    }
}

void MainWindow::on_add_clicked()
{
    if(!is_changed)
    {
        current->setRowCount(current->rowCount()+1);
        is_changed = true;
        current->setCurrentCell(current->rowCount()-1,0);
        ui->add_ok->show();
    }
    else {
        QMessageBox::warning(NULL, "WARNING","Only one line can be added at a time." , QMessageBox::Yes, QMessageBox::Yes);
    }
}

void MainWindow::on_submit_clicked()
{
    this->login();
}

void MainWindow::grant_add()
{
    int row = ui->grant->rowCount()-1;
    QTableWidget*cur_table = ui->grant;
    if(!is_changed||row!=cur_table->rowCount()-1)
    {
        return;
    }
    if(cur_table->item(row,0)&&cur_table->item(row,0)->text()==tr(""))
    {
        return;
    }
    if(cur_table->item(row,1)&&cur_table->item(row,1)->text()==tr(""))
    {
        return;
    }
    if(cur_table->item(row,2)&&cur_table->item(row,2)->text()==tr(""))
    {
        return;
    }
    if(cur_table->item(row,3)&&cur_table->item(row,3)->text()==tr(""))
    {
        return;
    }
    if(cur_table->item(row,4)&&cur_table->item(row,4)->text()==tr(""))
    {
        return;
    }
        string values = "(\'"+cur_table->item(row,0)->text().toStdString()+"\',\'"+cur_table->item(row,1)->text().toStdString()+"\',"+cur_table->item(row,2)->text().toStdString()+','+cur_table->item(row,3)->text().toStdString()+','+cur_table->item(row,4)->text().toStdString()+')';
        if(!cur_user->insert("tablegrant",values))
        {
            QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
        }
        else
        {
            ui->add_ok->hide();
            is_changed = false;
        }

}

void MainWindow::user_add()
{
    int row = ui->user->rowCount()-1;
    QTableWidget*cur_table = ui->user;
    if(!is_changed||row!=cur_table->rowCount()-1)
    {
        return;
    }
    if(cur_table->item(row,0)&&cur_table->item(row,0)->text()==tr(""))
    {
        return;
    }
    if(cur_table->item(row,1)&&cur_table->item(row,1)->text()==tr(""))
    {
        return;
    }

        string values = "(\'"+cur_table->item(row,0)->text().toStdString()+"\',\'"+cur_table->item(row,1)->text().toStdString()+"\')";
        if(!cur_user->insert("user",values))
        {
            QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);

        }
        else {
            ui->add_ok->hide();
            is_changed = false;
        }
}

void MainWindow::client_add()
{
    int row = ui->client->rowCount()-1;
    QTableWidget*cur_table = ui->client;
    if(!is_changed||row!=cur_table->rowCount()-1)
    {
        return;
    }
    if(cur_table->item(row,0)&&cur_table->item(row,0)->text()==tr(""))
    {
        return;
    }
    if(cur_table->item(row,1)&&cur_table->item(row,1)->text()==tr(""))
    {
        return;
    }
    if(cur_table->item(row,2)&&cur_table->item(row,2)->text()==tr(""))
    {
        return;
    }

        string values = "("+cur_table->item(row,0)->text().toStdString()+",\'"+cur_table->item(row,1)->text().toStdString()+"\',\'"+cur_table->item(row,2)->text().toStdString()+"\')";
        if(!cur_user->insert("client",values))
        {
            QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
        }
        else
        {
            ui->add_ok->hide();
            is_changed = false;
        }
}

void MainWindow::staff_add()
{
    int row = ui->staff->rowCount()-1;
    QTableWidget*cur_table = ui->staff;
    if(!is_changed||row!=cur_table->rowCount()-1)
    {
        return;
    }
    if(cur_table->item(row,0)&&cur_table->item(row,0)->text()==tr(""))
    {
        return;
    }
    if(cur_table->item(row,1)&&cur_table->item(row,1)->text()==tr(""))
    {
        return;
    }

        string values = "("+cur_table->item(row,0)->text().toStdString()+",\'"+cur_table->item(row,1)->text().toStdString()+"\')";
        if(!cur_user->insert("staff",values))
        {
            QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
        }
        else {
            ui->add_ok->hide();
            is_changed = false;
        }
}

void MainWindow::supplier_add()
{
    int row = ui->supplier->rowCount()-1;
    QTableWidget*cur_table = ui->supplier;
    if(!is_changed||row!=cur_table->rowCount()-1)
    {
        return;
    }
    if(cur_table->item(row,0)&&cur_table->item(row,0)->text()==tr(""))
    {
        return;
    }
    if(cur_table->item(row,1)&&cur_table->item(row,1)->text()==tr(""))
    {
        return;
    }
    if(cur_table->item(row,2)&&cur_table->item(row,2)->text()==tr(""))
    {
        return;
    }
    if(cur_table->item(row,3)&&cur_table->item(row,3)->text()==tr(""))
    {
        return;
    }

        string values = "("+cur_table->item(row,0)->text().toStdString()+",\'"+cur_table->item(row,1)->text().toStdString()+"\',\'"+cur_table->item(row,2)->text().toStdString()+"\',\'"+cur_table->item(row,3)->text().toStdString()+"\')";
        if(!cur_user->insert("supplier",values))
        {
            QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
        }
        else {
            ui->add_ok->hide();
            is_changed = false;
        }
}

void MainWindow::medicine_add()
{
    int row = ui->medicine->rowCount()-1;
    QTableWidget*cur_table = ui->medicine;
    if(!is_changed||row!=cur_table->rowCount()-1)
    {
        return;
    }
    if(cur_table->item(row,0)&&cur_table->item(row,0)->text()==tr(""))
    {
        return;
    }
    if(cur_table->item(row,1)&&cur_table->item(row,1)->text()==tr(""))
    {
        return;
    }
    if(cur_table->item(row,2)&&cur_table->item(row,2)->text()==tr(""))
    {
        return;
    }
    if(cur_table->item(row,3)&&cur_table->item(row,3)->text()==tr(""))
    {
        return;
    }
    if(cur_table->item(row,4)&&cur_table->item(row,4)->text()==tr(""))
    {
        return;
    }

        string values = "("+cur_table->item(row,0)->text().toStdString()+",\'"+cur_table->item(row,1)->text().toStdString()+"\',"+cur_table->item(row,2)->text().toStdString()+","+cur_table->item(row,3)->text().toStdString()+","+cur_table->item(row,4)->text().toStdString()+")";
        if(!cur_user->insert("medicine",values))
        {
            QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
        }
        else {
            ui->add_ok->hide();
            is_changed = false;
        }
}



void MainWindow::warehouse_add()
{
    int row = ui->warehouse->rowCount()-1;
    QTableWidget*cur_table = ui->warehouse;
    if(!is_changed||row!=cur_table->rowCount()-1)
    {
        return;
    }
    if(cur_table->item(row,0)&&cur_table->item(row,0)->text()==tr(""))
    {
        return;
    }
    if(cur_table->item(row,2)&&cur_table->item(row,2)->text()==tr(""))
    {
        return;
    }
    string select_query = "warehouse.waremid="+cur_table->item(row,0)->text().toStdString();
    if(cur_user->select("warehouse","*",select_query))
    {
        if(mysql_num_rows(cur_user->get_result())>0)
        {
            if(!cur_user->update("warehouse","stocknum=stocknum+"+cur_table->item(row,2)->text().toStdString(),select_query))
                QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
            else
            {
                ui->add_ok->hide();
                is_changed = false;
                ui->query->click();
            }
        }
        else
        {
        string values = "("+cur_table->item(row,0)->text().toStdString()+","+cur_table->item(row,2)->text().toStdString()+")";
        if(!cur_user->insert("warehouse",values))
        {
            QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
        }
        else {
            ui->add_ok->hide();
            is_changed = false;
        }
        }
    }
    else
        QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
}

void MainWindow::sales_add()
{
    int row = ui->sales->rowCount()-1;
    QTableWidget*cur_table = ui->sales;
    if(!is_changed||row!=cur_table->rowCount()-1)
    {
        return;
    }
    if(cur_table->item(row,0)&&cur_table->item(row,0)->text()==tr(""))
    {
        return;
    }
    if(cur_table->item(row,2)&&cur_table->item(row,2)->text()==tr(""))
    {
        return;
    }
    if(cur_table->item(row,4)&&cur_table->item(row,4)->text()==tr(""))
    {
        return;
    }

        string values = "("+cur_table->item(row,0)->text().toStdString()+","+cur_table->item(row,2)->text().toStdString()+","+cur_table->item(row,4)->text().toStdString()+",date_format(now(),'%y-%m-%d'))";
        if(!cur_user->insert("sales",values))
        {
            QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
        }
        else {
            ui->add_ok->hide();
            is_changed = false;
        }
}


void MainWindow::on_add_ok_clicked()
{
    QString cur_table_name = ui->comboBox->currentText();
    if(QString::compare(cur_table_name,"user")==0)
    {
        this->user_add();
    }
    else if(QString::compare(cur_table_name,"tablegrant")==0)
    {
        this->grant_add();
    }
    else if(QString::compare(cur_table_name,"medicine")==0)
    {
        this->medicine_add();
    }
    else if(QString::compare(cur_table_name,"client")==0)
    {
        this->client_add();
    }
    else if(QString::compare(cur_table_name,"supplier")==0)
    {
        this->supplier_add();
    }
    else if(QString::compare(cur_table_name,"staff")==0)
    {
        this->staff_add();
    }
    else if(QString::compare(cur_table_name,"sales")==0)
    {
        this->sales_add();
    }
    else if(QString::compare(cur_table_name,"warehouse")==0)
    {
        this->warehouse_add();
    }
}

void MainWindow::on_select_clicked()
{
    QString cur_table_name = ui->comboBox->currentText();
    QString query_filter = ui->filter->text();
    if(query_filter==tr(""))return;
    if(QString::compare(cur_table_name,"user")==0)
    {
        int index = query_filter.indexOf("Username");
        if(index!=-1)
            query_filter.replace(index,8,"user.username");
        index = query_filter.indexOf("Password");
        if(index!=-1)
            query_filter.replace(index,8,"user.password");
        if(cur_user->select("user","*",query_filter.toStdString()))
        {
            current->clearContents();
        }
        else {
            QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
    }
    else if(QString::compare(cur_table_name,"tablegrant")==0)
    {
        int index = query_filter.indexOf("Username");
        if(index!=-1)
            query_filter.replace(index,8,"tablegrant.username");
        index = query_filter.indexOf("Tablename");
        if(index!=-1)
            query_filter.replace(index,9,"tablegrant.tablename");
        index = query_filter.indexOf("Readable");
        if(index!=-1)
            query_filter.replace(index,8,"tablegrant.pread");
        index = query_filter.indexOf("Writable");
        if(index!=-1)
            query_filter.replace(index,8,"tablegrant.pwrite");
        index = query_filter.indexOf("Deletable");
        if(index!=-1)
            query_filter.replace(index,9,"tablegrant.pdelete");
        if(cur_user->select("tablegrant","*",query_filter.toStdString()))
        {
            current->clearContents();
        }
        else {
            QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
    }
    else if(QString::compare(cur_table_name,"medicine")==0)
    {
        int index = query_filter.indexOf("MedicineID");
        if(index!=-1)
            query_filter.replace(index,10,"medicine.mid");
        index = query_filter.indexOf("Name");
        if(index!=-1)
            query_filter.replace(index,4,"medicine.name");
        index = query_filter.indexOf("SalesPrice");
        if(index!=-1)
            query_filter.replace(index,10,"medicine.salesprice");
        index = query_filter.indexOf("PruchasePrice");
        if(index!=-1)
            query_filter.replace(index,13,"medicine.purchaseprice");
        index = query_filter.indexOf("SupplierID");
        if(index!=-1)
            query_filter.replace(index,10,"medicine.sid");
        index = query_filter.indexOf("SupplierName");
        if(index!=-1)
            query_filter.replace(index,12,"supplier.sname");
        if(cur_user->select("medicine,supplier","medicine.mid,medicine.name,medicine.salesprice,medicine.purchaseprice,medicine.sid,supplier.sname","medicine.sid=supplier.sid and "+query_filter.toStdString()))
        {
            current->clearContents();
        }
        else {
            QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
    }
    else if(QString::compare(cur_table_name,"client")==0)
    {
        int index = query_filter.indexOf("ClientID");
        if(index!=-1)
            query_filter.replace(index,8,"client.cid");
        index = query_filter.indexOf("Name");
        if(index!=-1)
            query_filter.replace(index,4,"client.name");
        index = query_filter.indexOf("Phone");
        if(index!=-1)
            query_filter.replace(index,5,"client.phone");
        if(cur_user->select("client","*",query_filter.toStdString()))
        {
            current->clearContents();
        }
        else {
            QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
    }
    else if(QString::compare(cur_table_name,"supplier")==0)
    {
        int index = query_filter.indexOf("SupplierID");
        if(index!=-1)
            query_filter.replace(index,10,"supplier.sid");
        index = query_filter.indexOf("Name");
        if(index!=-1)
            query_filter.replace(index,4,"supplier.sname");
        index = query_filter.indexOf("Phone");
        if(index!=-1)
            query_filter.replace(index,5,"supplier.phone");
        index = query_filter.indexOf("City");
        if(index!=-1)
            query_filter.replace(index,4,"supplier.city");
         if(cur_user->select("supplier","*",query_filter.toStdString()))
         {
             current->clearContents();
         }
         else {
             QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
             return;
         }

    }
    else if(QString::compare(cur_table_name,"staff")==0)
    {
        int index = query_filter.indexOf("StaffID");
        if(index!=-1)
            query_filter.replace(index,7,"staff.staffid");
        index = query_filter.indexOf("Name");
        if(index!=-1)
            query_filter.replace(index,4,"staff.staffname");
        if(cur_user->select("staff","*",query_filter.toStdString()))
        {
            current->clearContents();
        }
        else {
            QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
    }
    else if(QString::compare(cur_table_name,"sales")==0)
    {
        int index = query_filter.indexOf("MedicineID");
        if(index!=-1)
            query_filter.replace(index,10,"sales.medicineid");
        index = query_filter.indexOf("MedicineName");
        if(index!=-1)
            query_filter.replace(index,12,"medicine.name");
        index = query_filter.indexOf("StaffID");
        if(index!=-1)
            query_filter.replace(index,7,"sales.staffid");
        index = query_filter.indexOf("StaffName");
        if(index!=-1)
            query_filter.replace(index,9,"staff.staffname");
        index = query_filter.indexOf("ClientID");
        if(index!=-1)
            query_filter.replace(index,8,"sales.clientid");
        index = query_filter.indexOf("ClientName");
        if(index!=-1)
            query_filter.replace(index,10,"client.name");
        index = query_filter.indexOf("Date");
        if(index!=-1)
            query_filter.replace(index,4,"sales.curdate");
        if(cur_user->select("sales,medicine,staff,client","sales.medicineid,medicine.name,sales.staffid,staff.staffname,sales.clientid,client.name,sales.curdate","sales.medicineid=medicine.mid and sales.staffid=staff.staffid and sales.clientid=client.cid and "+query_filter.toStdString()))
        {
            current->clearContents();
        }
        else {
            QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
    }
    else if(QString::compare(cur_table_name,"warehouse")==0)
    {
        int index = query_filter.indexOf("MedicineID");
        if(index!=-1)
            query_filter.replace(index,10,"warehouse.waremid");
        index = query_filter.indexOf("MedicineName");
        if(index!=-1)
            query_filter.replace(index,12,"medicine.name");
        index = query_filter.indexOf("StockNum");
        if(index!=-1)
            query_filter.replace(index,8,"warehouse.stocknum");
        if(cur_user->select("warehouse,medicine","warehouse.waremid,medicine.name,warehouse.stocknum","warehouse.waremid=medicine.mid and "+query_filter.toStdString()))
        {
            current->clearContents();
        }
        else {
            QMessageBox::warning(NULL, "WARNING",QString::fromStdString(cur_user->get_error()) , QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
    }
    this->data_print();
}
