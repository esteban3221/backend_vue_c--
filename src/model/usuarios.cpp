#include "usuarios.hpp"

namespace model
{
    /// @brief ===============Atomicas====================
    Gtk::TreeView *tree_usuarios = nullptr;
    Glib::RefPtr<Gtk::ListStore> ModelUsuarios = nullptr;
    Model::Usuarios_Config m_Colunms_usuarios;
    //==================================================
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
    std::string usuarios::altaUsuario(const std::string &user, const std::string &passw)
    {
        this->sqlite3->command("SELECT * FROM usuarios WHERE username = ? LIMIT 1", user.c_str());
        if(this->sqlite3->get_result()["id"].size() > 0)
            throw std::runtime_error("El usuario ya existe");
        this->sqlite3->command("insert into usuarios(username, password) values (?, ?)", user.c_str(), passw.c_str());
        this->sqlite3->command("select id from usuarios ORDER BY id DESC");
        return this->sqlite3->get_result()["id"][0];
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
        this->sqlite3->command("update usuarios set password = ? where username = ?", passw.c_str(), user.c_str());
        return this->sqlite3->get_rc();
    }
    std::map<std::string, std::vector<std::string>> usuarios::obten_usuarios()
    {
        this->sqlite3->command("select id, username from usuarios");
        return this->sqlite3->get_result();
    }
} // namespace model
