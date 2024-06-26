#pragma once
#include "conectbd.hpp"
#include <vector>
#include <map>

namespace Model
{
    class usuarios_roles : public connectSqlite
    {
    public:
        usuarios_roles(/* args */);
        ~usuarios_roles();
        std::map<std::string, std::vector<std::string>> obten_roles_by_id_usuarios(const std::string &id);
        std::map<std::string, std::vector<std::string>> obten_roles_by_usuarios(const std::string &username);
    };
} // namespace Model
