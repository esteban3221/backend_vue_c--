#include "view_five.hpp"

void view_five::init_widgets()
{
    for (int i = 0 ; i < 19; i++)
    {
        this->ArrayFrameButton[i] = this->builder->get_widget<Gtk::CheckButton>("check_rol_" + std::to_string(i+1));
        this->ArrayFrameButton[i]->set_sensitive(false);
    }
    model::tree_usuarios = this->builder->get_widget<Gtk::TreeView>("tree_usuarios");
    model::usuarios M_usuarios; 
    
    auto contenedor_usuarios = M_usuarios.obten_usuarios();

    model::ModelUsuarios = Gtk::ListStore::create(model::m_Colunms_usuarios);
    model::tree_usuarios->set_model(model::ModelUsuarios);

    model::tree_usuarios->append_column("Usuario", model::m_Colunms_usuarios.nombre);
     
    for (size_t i = 0; i < contenedor_usuarios["id"].size(); i++)
    {
        this->row = *(model::ModelUsuarios->append());
        row[model::m_Colunms_usuarios.id] = contenedor_usuarios["id"][i];
        row[model::m_Colunms_usuarios.nombre] = contenedor_usuarios["username"][i];
    }
    model::tree_usuarios->set_activate_on_single_click();
    model::tree_usuarios->signal_row_activated().connect(sigc::mem_fun(*this, &view_five::on_tree_detalle_usuario_row_activated));
}

void view_five::on_tree_detalle_usuario_row_activated(const Gtk::TreeModel::Path &path, Gtk::TreeViewColumn *column)
{
    auto iter = model::ModelUsuarios->get_iter(path);
    if(iter)
    {
        Model::usuarios_roles M_usuarios_roles;

        for(const auto &i : this->ArrayFrameButton)  
            i->set_active(false);

        auto contenedor_roles = M_usuarios_roles.obten_roles_by_id_usuarios((*iter)[model::m_Colunms_usuarios.id].operator Glib::ustring())["id_rol"];
        
        for(auto &&i : contenedor_roles)
            this->ArrayFrameButton[std::stoi(i) - 1]->set_active();
    }
}

view_five::view_five(/* args */)
{
    builder->add_from_string(this->PAGE4);
    this->page_4 = this->builder->get_widget<Gtk::Box>("page_4");
    this->append(*page_4);
    this->page_4->set_hexpand();

    this->init_widgets();
}

view_five::~view_five()
{
}

