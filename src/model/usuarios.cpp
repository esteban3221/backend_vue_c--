#include "usuarios.hpp"

namespace model
{
    usuarios::usuarios()
    {
    }

    usuarios::~usuarios()
    {
    }
    bool usuarios::validaUsuario(const std::string &user)
    {
        this->sqlite3->command("select * from usuarios where username = ?", user);
        return not this->sqlite3->get_result().empty();
    }
    bool usuarios::validaUsuario(const std::string &user, const std::string &passw)
    {
        // old
        // this->sqlite3->command("select * from usuarios where username = '" + user + "' and password = '" + passw + "'");

        // prepared
        this->sqlite3->command("select * from usuarios where username = ? and password = ?", user.c_str(), passw.c_str());

        return not this->sqlite3->get_result().empty();
    }
    void usuarios::altaUsuario(const std::string &user, const std::string &passw)
    {
        this->sqlite3->command("insert into usuarios(username, password) values (?, ?)", user.c_str(), passw.c_str());
    }
    void usuarios::bajaUsuario(const std::string &user)
    {
        this->sqlite3->command("delete from usuarios where username = ?", user.c_str());
    }
    void usuarios::bajaUsuario(const unsigned &id)
    {
        this->sqlite3->command("delete from usuarios where id = ?", id);
    }
    bool usuarios::modificaUsuario(const std::string &user, const std::string &passw)
    {
        // por saber si nos enviaran un username o un id
        return true;
    }
    std::map<std::string, std::vector<std::string>> usuarios::obten_usuarios()
    {
        this->sqlite3->command("select id, username from usuarios");
        return this->sqlite3->get_result();
    }
} // namespace model
