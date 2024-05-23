#include "view_three.hpp"

view_three::view_three(/* args */)
{
    builder->add_from_string(this->PAGE2);
    this->page_2 = this->builder->get_widget<Gtk::Box>("page_2");
    this->inti_ety_datos();
    this->append(*page_2);
    this->page_2->set_hexpand();
    
}

view_three::~view_three()
{
}

void view_three::inti_ety_datos()
{
    this->list_ety_datos[0] = builder->get_widget<Gtk::Entry>("ety_conf_razon");
    this->list_ety_datos[1] = builder->get_widget<Gtk::Entry>("ety_conf_direccion");
    this->list_ety_datos[2] = builder->get_widget<Gtk::Entry>("ety_conf_rfc");
    this->list_ety_datos[3] = builder->get_widget<Gtk::Entry>("ety_conf_contacto");
    this->list_ety_datos[4] = builder->get_widget<Gtk::Entry>("ety_conf_thanks");

    for (int i = 0; i < list_ety_datos.size(); i++)
        list_ety_datos[i]->set_text(BinaryDB::select_<std::string>(i + 2));
    for (int i = 0; i < list_ety_datos.size(); i++) 
        list_ety_datos[i]->signal_changed().connect([this, i]() 
        {  
            BinaryDB::update_(i + 2, list_ety_datos[i]->get_text().operator std::string()); 
            Config::TextView::preview_ticket();
        });
}