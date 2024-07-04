#include "view_two.hpp"

view_two::view_two(/* args */)
{
    this->builder->add_from_string(this->PAGE1);
    this->page_1 = this->builder->get_widget<Gtk::Box>("page_1");
    this->append(*page_1);
    this->page_1->set_hexpand();
    this->init_info_systema();
}

view_two::~view_two()
{
}

void view_two::init_info_systema()
{
    this->list_info_system[0] = builder->get_widget<Gtk::EditableLabel>("ety_info_host");
    this->list_info_system[1] = builder->get_widget<Gtk::EditableLabel>("ety_info_modelo");
    this->list_info_system[2] = builder->get_widget<Gtk::EditableLabel>("ety_info_procesador");
    this->list_info_system[3] = builder->get_widget<Gtk::EditableLabel>("ety_info_memoria");
    this->list_info_system[4] = builder->get_widget<Gtk::EditableLabel>("ety_info_disco");

    const std::string &parent{"cat /sys/devices/virtual/dmi/id/"};
    const std::string &a{parent + "board_vendor"};
    const std::string &b{parent + "product_name"};

    this->list_info_system[0]->set_text(Helper::System::exec("cat /etc/hostname"));
    this->list_info_system[1]->set_text((Helper::System::exec(a.c_str())).empty() ? 
    Helper::System::exec("cat /proc/device-tree/model") : 
    Helper::System::exec(a.c_str()) + " " + Helper::System::exec(b.c_str()));
    this->list_info_system[2]->set_text(Helper::System::exec("lscpu | grep -E 'Nombre del modelo|Model name' | awk -F': ' '{print $2}'"));
    this->list_info_system[3]->set_text(Helper::System::exec("grep MemTotal /proc/meminfo | awk '{print $2/1024/1024 \" GB\"}' "));
    this->list_info_system[4]->set_text(Helper::System::exec("lsblk -o SIZE -b | head -2 | tail -1 | awk '{print $1/1024/1024/1024 \" GB\"}'"));
}