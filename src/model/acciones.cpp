#include "acciones.hpp"

Acciones::Acciones(/* args */)
{
}

Acciones::~Acciones()
{
}

void Acciones::insertLog(const std::string &user,
                         const Action::Type &type,
                         const std::string &total,
                         const std::string &ingreso,
                         const std::string &cambio,
                         const std::string &status)
{
    this->sqlite3->command("insert into log VALUES(null, (Select id from usuarios where username = ?), ?, ? ,? ,? ,?, datetime('now','localtime'))",
                           user.c_str(),
                           Action::typeVal[static_cast<unsigned>(type)].c_str(),
                           ingreso.c_str(),
                           cambio.c_str(),
                           total.c_str(),
                           status.c_str());
}
