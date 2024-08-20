#pragma once
#include "conectbd.hpp"
#include "usuarios_config.hpp"
#include <utility>

namespace model
{
    extern Gtk::TreeView *tree_usuarios;
    extern Glib::RefPtr<Gtk::ListStore> ModelUsuarios;
    extern Model::Usuarios_Config m_Colunms_usuarios;
    class usuarios : public connectSqlite
    {
    private:
        /* data */
    public:
        usuarios(/* args */);
        ~usuarios();

        bool validaUsuario(const std::string &user, const std::string &passw);
        // bool validaUsuario(const std::string &user);
        std::map<std::string, std::vector<std::string>> validaUsuario(const std::string &passw);
        std::string altaUsuario(const std::string &user,const std::string &passw);
        void bajaUsuario(const std::string &user);
        void bajaUsuario(const unsigned &id);
        bool modificaUsuario(const std::string &user, const std::string &passw);
        std::map<std::string, std::vector<std::string>> obten_usuarios();
    };
} // namespace model
