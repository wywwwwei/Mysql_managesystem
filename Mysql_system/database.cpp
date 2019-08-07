#include"Database.hpp"

string Database::get_error()
{
    return string(mysql_error(&mysql));
}

MYSQL_RES* Database::get_result()
{
    return this->result;
}

bool Database::connect(string server, string user, string password, string schema, int port)
{
    mysql_init(&mysql);
    if (mysql_real_connect(&mysql, server.c_str(), user.c_str(), password.c_str(), schema.c_str(), port, 0, 0))
    {
        mysql_query(&mysql, "set names gbk");
        return true;
    }
    else
    {
        return false;
    }
}

bool Database::check_table(string tablename)
{
    result = mysql_list_tables(&mysql, tablename.c_str());
    return (mysql_num_rows(result) > 0);
}

bool Database::insert(string query)
{
    return (mysql_query(&mysql, query.c_str())==0);
}

bool Database::select(string query)
{
    if (mysql_query(&mysql, query.c_str())==0)
    {
        result = mysql_store_result(&mysql);
        return true;
    }
    return false;
}

bool Database::update(string query)
{
    return (mysql_query(&mysql, query.c_str())==0);
}

bool Database::exec(string query)
{
    return (mysql_query(&mysql, query.c_str()) == 0);
}

bool Database::disconnect()
{
    mysql_close(&mysql);
}
