#include"Controller.hpp"
#include<QMessageBox>
bool Controller::init()
{
    if (manage_system == nullptr)
    {
        manage_system = new Database();
    }
    if (!manage_system->connect("localhost", "root", "1191448318", "temp", 3306))
    {
        return false;
    }
    if (!manage_system->check_table("user"))
    {
        string query = "CREATE TABLE `user` (\
            `username` VARCHAR(20) NOT NULL,\
            `password` VARCHAR(20) NOT NULL,\
            PRIMARY KEY(`username`))";
        if (!manage_system->exec(query))
            return false;
        query.clear();
        query = "INSERT INTO `user` (`username`, `password`) VALUES ('admin', 'admin')";
        if (!manage_system->exec(query))
            return false;
    }
    if (!manage_system->check_table("supplier"))
    {
        string query = "CREATE TABLE `supplier` (\
            `sid` INT NOT NULL,\
            `sname` VARCHAR(45) NOT NULL,\
            `phone` VARCHAR(25) NOT NULL,\
            `city` VARCHAR(45) NOT NULL,\
            PRIMARY KEY(`sid`))";
        if (!manage_system->exec(query))
            return false;
        query.clear();
        query = "INSERT INTO `supplier` (`sid`, `sname`, `phone`, `city`) VALUES ('1', 'SYSU', '12345678901', 'GZ');";
        if (!manage_system->exec(query))
            return false;
    }
    if (!manage_system->check_table("staff"))
    {
        string query = "CREATE TABLE `staff` (\
            `staffid` INT NOT NULL,\
            `staffname` VARCHAR(45) NOT NULL,\
            PRIMARY KEY(`staffid`))";
        if (!manage_system->exec(query))
            return false;
        query.clear();
        query = "INSERT INTO `staff` (`staffid`, `staffname`) VALUES ('1', 'WYW')";
        if (!manage_system->exec(query))
            return false;
    }
    if (!manage_system->check_table("client"))
    {
        string query = "CREATE TABLE `client` (\
            `cid` INT NOT NULL,\
            `name` VARCHAR(45) NOT NULL,\
            `phone` VARCHAR(45) NOT NULL,\
            PRIMARY KEY(`cid`))";
        if (!manage_system->exec(query))
            return false;
        query.clear();
        query = "INSERT INTO `client` (`cid`, `name`, `phone`) VALUES ('1', 'Wu', '12345678910')";
        if (!manage_system->exec(query))
            return false;
    }
    if (!manage_system->check_table("tablegrant"))
    {
        string query = "CREATE TABLE `tablegrant` (\
            `username` VARCHAR(20) NOT NULL,\
            `tablename` VARCHAR(45) NOT NULL,\
            `pread` TINYINT(1) NOT NULL,\
            `pwrite` TINYINT(1) NOT NULL,\
            `pdelete` TINYINT(1) NOT NULL,\
            PRIMARY KEY (`username`, `tablename`),\
                CONSTRAINT `username`\
                FOREIGN KEY(`username`)\
                    REFERENCES `user` (`username`)\
                        ON DELETE CASCADE\
                        ON UPDATE CASCADE)";
        if (!manage_system->exec(query))
            return false;
        query.clear();
        query = "INSERT INTO `tablegrant` (`username`, `tablename`, `pread`, `pwrite`, `pdelete`) VALUES ('admin', 'user', '1', '1', '1'),\
        ('admin', 'supplier', '1', '1', '1'),\
        ('admin', 'staff', '1', '1', '1'),\
        ('admin', 'client', '1', '1', '1'),\
        ('admin', 'tablegrant', '1', '1', '1'),\
        ('admin', 'sales', '1', '1', '1'),\
        ('admin', 'medicine', '1', '1', '1'),\
        ('admin', 'warehouse', '1', '1', '1')";
        if (!manage_system->exec(query))
            return false;
    }
    if (!manage_system->check_table("medicine"))
    {
        string query = "CREATE TABLE `medicine` (\
            `mid` INT NOT NULL,\
            `name` VARCHAR(45) NOT NULL,\
            `salesprice` INT NOT NULL,\
            `purchaseprice` INT NOT NULL,\
            `sid` INT(11) NOT NULL,\
            PRIMARY KEY(`mid`,`sid`),\
                INDEX `sid_idx` (`sid` ASC) VISIBLE,\
                CONSTRAINT `sid`\
                FOREIGN KEY(`sid`)\
                    REFERENCES `supplier` (`sid`)\
                        ON DELETE NO ACTION\
                        ON UPDATE NO ACTION)";
        if (!manage_system->exec(query))
            return false;
        query.clear();
        query = "INSERT INTO `medicine` (`mid`, `name`, `salesprice`, `purchaseprice`, `sid`) VALUES ('1', 'AB', '10', '9', '1');";
        if (!manage_system->exec(query))
            return false;
    }
    if (!manage_system->check_table("sales"))
    {
        string query = "CREATE TABLE `sales` (\
            `medicineid` INT NOT NULL,\
            `staffid` INT(11) NOT NULL,\
            `clientid` INT(11) NOT NULL,\
            `curdate` DATE NOT NULL,\
            PRIMARY KEY (`medicineid`, `staffid`, `clientid`),\
                INDEX `staffid_idx` (`staffid` ASC) VISIBLE,\
                INDEX `clientid_idx` (`clientid` ASC) VISIBLE,\
                CONSTRAINT `medicineid`\
                FOREIGN KEY(`medicineid`)\
                    REFERENCES `medicine` (`mid`)\
                        ON DELETE NO ACTION\
                        ON UPDATE NO ACTION,\
                        CONSTRAINT `staffid`\
                        FOREIGN KEY(`staffid`)\
                            REFERENCES `staff` (`staffid`)\
                                ON DELETE NO ACTION\
                                ON UPDATE NO ACTION,\
                                CONSTRAINT `clientid`\
                                FOREIGN KEY(`clientid`)\
                                    REFERENCES `client` (`cid`)\
                                        ON DELETE NO ACTION\
                                        ON UPDATE NO ACTION);";
        if (!manage_system->exec(query))
            return false;
        query.clear();
        query = "INSERT INTO `sales` (`medicineid`, `staffid`, `clientid`,`curdate`) VALUES ('1', '1', '1',date_format(now(),'%y-%m-%d'));";
        if (!manage_system->exec(query))
            return false;
    }
    if (!manage_system->check_table("warehouse"))
    {
        string query = "CREATE TABLE `warehouse` (\
            `waremid` INT NOT NULL,\
            `stocknum` INT NOT NULL,\
            PRIMARY KEY(`waremid`),\
                CONSTRAINT `waremid`\
                FOREIGN KEY(`waremid`)\
                    REFERENCES `medicine` (`mid`)\
                        ON DELETE CASCADE\
                        ON UPDATE CASCADE);";
        if (!manage_system->exec(query))
            return false;
        query.clear();
        query = "INSERT INTO `warehouse` (`waremid`, `stocknum`) VALUES ('1', '10');";
        if (!manage_system->exec(query))
            return false;
    }
    return true;
}

string Controller::get_error()
{
    return manage_system->get_error();
}

MYSQL_RES* Controller::get_result()
{
    return manage_system->get_result();
}

bool Controller::login(string username, string password)
{
    string query = "select * from user where username=\'" + username + "\' and password=\'" + password+'\'';
    if (manage_system->select(query))
    {
        if (mysql_num_rows(this->get_result()) > 0)
        {
            this->username = username;
            this->password = password;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool Controller::logout()
{
    this->username.clear();
    this->password.clear();
    this->grant.clear();
    return true;
}

bool Controller::select(string table_name, string field, string filter)
{
     string query;
    if(filter.length()!=0)
        query = "select " + field + " from " + table_name + " where " + filter;
    else
        query = "select " + field + " from " + table_name ;
    return (manage_system->select(query));
}

bool Controller::insert(string table_name, string values)
{
    string query = "Insert into "+table_name+" values "+values;
    return (manage_system->insert(query));
}

bool Controller::update(string tablename, string field, string filter)
{
    string query = "update " + tablename + " set " + field + " where " + filter;
    return (manage_system->update(query));
}

bool Controller::getGrantUpdate()
{
    string filter = "username=\'" + this->username + '\'';
    if (!this->select("tablegrant", "*", filter))
    {
        return false;
    }
    MYSQL_RES *res = get_result();
    MYSQL_ROW row_data;

    while (row_data = mysql_fetch_row(res))
    {
        bool *cores_grant = new bool[3];
        string temp = row_data[1];
        if(*(row_data[2])=='1')
            cores_grant[0] = true;
        else
            cores_grant[0] = false;
        if(*(row_data[3])=='1')
            cores_grant[1] = true;
        else
            cores_grant[1] = false;
        if(*(row_data[4])=='1')
            cores_grant[2] = true;
        else
            cores_grant[2] = false;
        grant.emplace(pair<string, bool*>(row_data[1], cores_grant));
    }
    return true;
}

bool* Controller::curGrant(string tablename)
{
    if (grant.count(tablename) == 0)
        return nullptr;
    return grant[tablename];
}

void Controller::setusername(string username)
{
    this->username = username;
}

bool Controller::delete_item(string tablename,string filter)
{
    string query = "delete from "+tablename+" where "+filter;
    return manage_system->exec(query);
}
