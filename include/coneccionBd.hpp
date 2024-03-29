#pragma once
#include <pqxx/pqxx>
#include <pqxx/except>
#include <utility>
#include <iostream>

class coneccionBd
{
private:
    pqxx::connection conexion;

public:
    coneccionBd(const std::string &dbname, const std::string &user, const std::string &password, const std::string &hostaddr = "127.0.0.1", const std::string &port = "5432");
    ~coneccionBd();

    const pqxx::result query(const std::string &sql);

    // template <typename... Args>
    // const pqxx::result query_prepare(const std::string &sql, Args &&...args)
    // {
    //     pqxx::work consulta(this->conexion);
    //     try
    //     {
    //         return consulta.exec_prepared(sql, std::forward<Args>(args)...);
    //     }
    //     catch (pqxx::sql_error const &e)
    //     {
    //         std::cout << e.query() << "\n"
    //                   << e.what() << "\n";
    //         throw(e.query() + "\n" + e.what() + "\n").c_str();
    //     }
    // }
};
