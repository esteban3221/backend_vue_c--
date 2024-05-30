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
} // namespace Model
