#include "usuarios.hpp"

namespace model
{
    usuarios::usuarios()
    {
    }

    usuarios::~usuarios()
    {
    }
    bool usuarios::validaUsuario(const std::string &user){
        this->sqlite3->command("select * from usuarios where username = '" + user + "'");
        return not this->sqlite3->get_result().empty();
    }
    bool usuarios::validaUsuario(const std::string &user, const std::string &passw)
    {
        this->sqlite3->command("select * from usuarios where username = '" + user + "' and password = '" + passw + "'");
        return not this->sqlite3->get_result().empty();
    }
    void usuarios::altaUsuario(const std::string &user, const std::string &passw)
    {
        this->sqlite3->command("insert into usuarios(username, password) values ('" + user + "', '" + passw + "')");
    }
    void usuarios::bajaUsuario(const std::string &user)
    {
        this->sqlite3->command("delete from usuarios where username = '" + user + "'");
    }
    bool usuarios::modificaUsuario(const std::string &user, const std::string &passw)
    {
        //por saber si nos enviaran un username o un id
        return true;
    }
} // namespace model