#include <pqxx/pqxx>
#include <pqxx/except>

class coneccionBd
{
private:
    pqxx::connection conexion;

public:
    coneccionBd(const std::string &dbname, const std::string &user, const std::string &password, const std::string &hostaddr, const std::string &port);
    ~coneccionBd();

    const pqxx::result query(const std::string &sql) ;
};
