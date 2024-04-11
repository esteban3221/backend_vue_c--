#include "sqlite.hpp"
#include <memory>

class connectSqlite
{
private:

public:
    std::shared_ptr<SQLite3::SQLite> sqlite3 = std::make_shared<SQLite3::SQLite>("maxicajero.db");
    connectSqlite(/* args */){std::cout <<  sqlite3->open() ? "OK DB\n": "Error Fatal \n";}
    ~connectSqlite(){}
};

