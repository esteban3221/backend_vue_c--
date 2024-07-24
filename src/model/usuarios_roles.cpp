#include "usuarios_roles.hpp"

namespace Model
{
    usuarios_roles::usuarios_roles(/* args */)
    {
    }

    usuarios_roles::~usuarios_roles()
    {
    }

    std::map<std::string, std::vector<std::string>> usuarios_roles::obten_roles_by_id_usuarios(const std::string &id)
    {
        this->sqlite3->command("SELECT id_rol FROM usuario_roles WHERE id_usuario = ?", id.c_str());
        return this->sqlite3->get_result();
    }

    std::map<std::string, std::vector<std::string>> usuarios_roles::obten_roles_by_usuarios(const std::string &username)
    {
        this->sqlite3->command("SELECT * FROM usuario_roles WHERE id_usuario = ("
                               "SELECT id FROM usuarios WHERE username = ?)",
                               username.c_str());
        return this->sqlite3->get_result();
    }
    void usuarios_roles::modificaRolesUsuario(const std::string &username, const std::vector<bool> &values)
    {
        this->sqlite3->command("DELETE FROM usuario_roles WHERE id_usuario = ("
                               "SELECT id FROM usuarios WHERE username = ?)",
                               username.c_str());

        for (size_t i = 0; i < 19; i++)
        {
            if(values[i])
                this->sqlite3->command("INSERT INTO usuario_roles values (NULL, (SELECT id FROM usuarios WHERE username = ?) , ?)", username.c_str(), i+1);
        }   
    }
} // namespace Model
