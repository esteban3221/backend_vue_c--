#include "view_five.hpp"

void view_five::init_widgets()
{
    for (int i = 0 ; i < 19; i++)
    {
        ArrayFrameButton[i] = this->builder->get_widget<Gtk::CheckButton>("check_rol_" + std::to_string(i+1));
        ArrayFrameButton[i]->set_sensitive(false);
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

