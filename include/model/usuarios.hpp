#pragma once
#include "conectbd.hpp"
#include <vector>

namespace model
{
    class usuarios : public connectSqlite
    {
    private:
        /* data */
    public:
        usuarios(/* args */);
        ~usuarios();

        bool validaUsuario(const std::string &user, const std::string &passw);
        bool validaUsuario(const std::string &user);
        void altaUsuario(const std::string &user,const std::string &passw);
        void bajaUsuario(const std::string &user);
        bool modificaUsuario(const std::string &user,const std::string &passw);
    };
} // namespace model
