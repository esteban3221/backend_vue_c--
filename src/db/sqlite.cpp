#include "sqlite.hpp"
#include <fstream>

namespace SQLite3
{
    SQLite::SQLite(const std::string &file_database_) : FILE_DATABASE(file_database_)
    {
        this->db = nullptr;
    }

    SQLite::~SQLite()
    {
        sqlite3_close(this->db);
    }

    const bool SQLite::open()
    {
        // std::ifstream file(this->FILE_DATABASE);

        // if (!file)
        // {
        //     this->rc = sqlite3_open(this->FILE_DATABASE.c_str(), &this->db);
        //     command("CREATE TABLE usuarios ("
        //             "id INTEGER PRIMARY KEY,"
        //             "username TEXT NOT NULL UNIQUE,"
        //             "password TEXT"
        //             ")");
        //     command("CREATE TABLE roles ("
        //             "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        //             "rol TEXT"
        //             ")");
        //     command("CREATE TABLE usuario_roles ("
        //             "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        //             "id_usuario INTEGER,"
        //             "id_rol INTEGER,"
        //             "FOREIGN KEY (id_usuario) REFERENCES usuarios (id),"
        //             "FOREIGN KEY (id_rol) REFERENCES roles (id)"
        //             ")");

        //     return true;

        this->rc = sqlite3_open(this->FILE_DATABASE.c_str(), &this->db);
        if (this->rc)
        {
            std::cout << "Error al abrir la base de datos: " << sqlite3_errmsg(this->db) << std::endl;
            return false;
        }
        else
        {
            std::cout << "Base de datos abierta correctamente" << std::endl;
            return true;
        }
    }

    inline bool SQLite::is_created()
    {
        std::ifstream file(this->FILE_DATABASE);
        return file ? true : false;
    }

    int SQLite::callback(void *NotUsed, int argc, char **argv, char **azColName)
    {
        for (size_t i = 0; i < argc; i++)
        {
            result[azColName[i]].emplace_back(argv[i] ? argv[i] : "");
        }
        return 0;
    }

    void SQLite::command(std::string sql)
    {
        result.clear();
        this->rc = sqlite3_exec(this->db, sql.c_str(), callback, 0, &zErrMsg);
        if (this->rc != SQLITE_OK)
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
    
    std::map<std::string, std::vector<std::string>> &SQLite::get_result() const
    {
        return result;
    }

    const int SQLite::get_rc() const
    {
        return this->rc;
    }
}