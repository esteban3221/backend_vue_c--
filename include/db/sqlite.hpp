#pragma once
#include <sqlite3.h>
#include <iostream>
#include <vector>

class SQLite
{
private:
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    std::string sql;
    std::string file_database;
    static int callback(void *, int , char **, char **);
    static inline std::vector<std::vector<std::string>> result;

public:
    SQLite(const std::string &file_database_);
    ~SQLite();
    bool open();
    void command(std::string);
    int get_rc() const;
    const std::vector<std::vector<std::string>> get_result() const;
};