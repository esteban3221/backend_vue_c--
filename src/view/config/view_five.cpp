#include "view_five.hpp"

view_five::view_five(/* args */)
{
    builder->add_from_string(this->PAGE4);
    this->page_4 = this->builder->get_widget<Gtk::Box>("page_4");
    this->append(*page_4);
    this->page_4->set_hexpand();
}

view_five::~view_five()
{
}

