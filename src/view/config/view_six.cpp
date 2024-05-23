#include "view_six.hpp"

void view_six::init_datos_red()
{
    this->lbl_red[0] = this->builder->get_widget<Gtk::EditableLabel>("ety_info_ip_wifi");
    this->lbl_red[1] = this->builder->get_widget<Gtk::EditableLabel>("ety_info_mac_wifi");
    this->lbl_red[2] = this->builder->get_widget<Gtk::EditableLabel>("ety_info_ip_eth");
    this->lbl_red[3] = this->builder->get_widget<Gtk::EditableLabel>("ety_info_ip_eth_mac");
}

view_six::view_six(/* args */)
{
    builder->add_from_string(this->PAGE5);
    this->page_5 = this->builder->get_widget<Gtk::Box>("page_5");
    this->append(*page_5);
    this->page_5->set_hexpand();
}

view_six::~view_six()
{
}
