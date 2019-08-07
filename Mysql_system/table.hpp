#ifndef TABLE_HPP
#define TABLE_HPP

#include<string>
#include "mysql.h"

using namespace std;


class User
{
    string username;
    string password;
};

class TableGrant
{
    string tablename;
    string username;
    bool pread;
    bool pwrite;
    bool pdelete;
};

class Medicine
{
    int id;
    string name;
    int sales_price;
    int purchase_price;
    int supplier_id;
};

class Staff
{
    int sid;
    string name;
};

class Client
{
    int cid;
    string name;
    string phone_number;
};

class Supplier
{
    int sid;
    string sname;
    string phone_number;
    string city;
};

class Warehouse
{
    int medicine_id;
    string medicine_name;
    int stock_num;
};

class Sales
{
    int medicine_id;
    string medicine_name;
    int staff_id;
    string staff_name;
    int client_id;
    string client_name;
    string curdate;
};

class Wholesale
{
    int medicine_id;
    string medicine_name;
    int supplier_id;
    int quantity;
};

#endif // TABLE_HPP
