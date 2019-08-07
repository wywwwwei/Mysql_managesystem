#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP
#include"Database.hpp"
#include<map>

class Controller
{
private:
    Database *manage_system;
    string username;
    string password;
    map<string, bool*>grant;
public:
    Controller(){manage_system=nullptr;}
    string get_error();
    MYSQL_RES* get_result();

    void setusername(string username);
    bool init();
    bool getGrantUpdate();
    bool login(string username, string password);
    bool logout();
    bool select(string table_name,string field,string filter);
    bool insert(string table_name, string values);
    bool update(string tablename, string field, string filter);
    bool* curGrant(string tablename);
    bool delete_item(string tablename,string filter);
    ~Controller()
    {
        manage_system->disconnect();
    }
};
#endif // CONTROLLER_HPP
