#pragma once
#include "conectbd.hpp"
#include "helper.hpp"

class Acciones : public connectSqlite
{
private:
    /* data */
public:
    Acciones(/* args */);
    ~Acciones();

    void insertLog(const std::string &user,
    const Action::Type &type,
    const std::string &total,
    const std::string &ingreso,
    const std::string &cambio,
    const std::string &status = "");
};
