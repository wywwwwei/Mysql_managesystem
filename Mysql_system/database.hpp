#ifndef DATABASE_HPP
#define DATABASE_HPP

#include"Table.hpp"

class Database
{
private:
    MYSQL mysql;
    MYSQL_RES *result;
public:
    string get_error();
    MYSQL_RES* get_result();
    bool connect(string server,string user,string password,string schema,int port);
    bool check_table(string tablename);
    bool insert(string query);
    bool select(string query);
    bool update(string query);
    bool exec(string query);
    bool disconnect();
};

#endif // DATABASE_HPP
