#pragma once
#include <gtkmm.h>

namespace Model
{
    class Usuarios_Config : public Gtk::TreeModel::ColumnRecord
    {
    public:
        Usuarios_Config(/* args */)
        {
            add(id);
            add(nombre);
        }
    
    Gtk::TreeModelColumn<Glib::ustring> id;
    Gtk::TreeModelColumn<Glib::ustring> nombre;
    };
    
} // namespace Model
