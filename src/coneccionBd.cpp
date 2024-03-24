#include "coneccionBd.hpp"
#include <iostream>

coneccionBd::coneccionBd(const std::string &dbname,
                         const std::string &user,
                         const std::string &password,
                         const std::string &hostaddr,
                         const std::string &port) : conexion("dbname=" + dbname +
                                                             " user=" + user +
                                                             " password=" + password +
                                                             " hostaddr=" + hostaddr +
                                                             " port=" + port)
{
    if (conexion.is_open())
    {
        std::cout << "Conexión exitosa a la base de datos PostgreSQL" << std::endl;
    }
    else
    {
        std::cout << "Error al conectar a la base de datos PostgreSQL" << std::endl;
        throw std::runtime_error("Error de conexión");
    }
}

coneccionBd::~coneccionBd()
{
    this->conexion.close();
    std::cout << "Desconexion a la base de datos PostgreSQL" << std::endl;
}

const pqxx::result coneccionBd::query(const std::string &sql)
{
    pqxx::work consulta(this->conexion);
    try
    {
        return consulta.exec(sql);
    }
    catch (pqxx::sql_error const &e)
    {
        std::cout << e.query() << "\n"<< e.what() << "\n";
        return {}; 
    }
}