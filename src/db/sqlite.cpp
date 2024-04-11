#include "sqlite.hpp"
#include <fstream>

namespace SQLite3
{
    SQLite::SQLite(const std::string &file_database_) : file_database(file_database_)
    {
        db = nullptr;
    }

    SQLite::~SQLite()
    {
        sqlite3_close(db);
    }

    bool SQLite::open()
    {
        std::ifstream file(file_database);

        if (!file)
        {
            rc = sqlite3_open(file_database.c_str(), &db);
            command("CREATE TABLE usuarios ("
                    "id INTEGER PRIMARY KEY,"
                    "username TEXT NOT NULL UNIQUE,"
                    "password TEXT"
                    ")");
            command("CREATE TABLE roles ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "rol TEXT"
                    ")");
            command("CREATE TABLE usuario_roles ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "id_usuario INTEGER,"
                    "id_rol INTEGER,"
                    "FOREIGN KEY (id_usuario) REFERENCES usuarios (id),"
                    "FOREIGN KEY (id_rol) REFERENCES roles (id)"
                    ")");

            return true;
        }

        rc = sqlite3_open(file_database.c_str(), &db);
        if (rc)
        {
            std::cout << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        else
        {
            std::cout << "Base de datos abierta correctamente" << std::endl;
            return true;
        }
    }

    int SQLite::callback(void *NotUsed, int argc, char **argv, char **azColName)
    {
        std::vector<std::string> row;
        for (size_t i = 0; i < argc; i++)
        {
            row.emplace_back(argv[i] ? argv[i] : "NULL");
        }
        result.emplace_back(row);
        return 0;
    }

    void SQLite::command(const std::string &sql)
    {
        result.clear();
        rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
        if (rc != SQLITE_OK)
        {
            std::string error = "SQL error: " + std::string(zErrMsg) + "\nSQL: " + sql + "\n";
            sqlite3_free(zErrMsg);
            throw std::runtime_error(error);
        }
        else
        {
            std::cout << "Operacion realizada correctamente" << std::endl;
            std::cout << "SQL: " << sql << std::endl;
        }
    }

    const std::vector<std::vector<std::string>> &SQLite::get_result() const
    {
        return result;
    }

    int SQLite::get_rc() const
    {
        return rc;
    }
}