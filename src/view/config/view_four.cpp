#include "view_four.hpp"

view_four::view_four(/* args */)
{
    builder->add_from_string(this->PAGE3);
    this->page_3 = this->builder->get_widget<Gtk::Box>("page_3");
    this->append(*page_3);
    this->page_3->set_hexpand();
    this->init_signals_impresion();
    text_ticket->set_buffer(Config::TextView::m_refTextBuffer1);
    Config::TextView::preview_ticket();

}

view_four::~view_four()
{
}

void view_four::init_signals_impresion()
{
    this->text_ticket = this->builder->get_widget<Gtk::TextView>("text_ticket");

    Config::list_config_test = this->builder->get_widget<Gtk::ListBox>("list_config_test");
    this->list_config = builder->get_widget<Gtk::ListBox>("list_config");
    this->list_config_visualizacion = builder->get_widget<Gtk::ListBox>("list_config_visualizacion");

    Config::switch_impresion = builder->get_widget<Gtk::Switch>("switch_impresion");
    Config::switch_impresion->set_active(BinaryDB::select_<int>(7));

    this->list_config->signal_row_activated().connect([this](Gtk::ListBoxRow *row)
                                                      {
        Config::switch_impresion->get_active() ? Config::switch_impresion->set_active(false) : Config::switch_impresion->set_active();
        BinaryDB::update_<int>(7, Config::switch_impresion->get_active()); });

    Config::switch_impresion->property_active().signal_changed().connect([this](){ BinaryDB::update_<int>(7, Config::switch_impresion->get_active()); });

    for (size_t i = 0; i < Config::list_view_ticket.size(); i++)
    {
        Config::list_view_ticket[i] = builder->get_widget<Gtk::CheckButton>("check_config_" + std::to_string(i + 1));
        Config::list_view_ticket[i]->set_active(BinaryDB::select_<int>(i + 10));
        Config::list_view_ticket[i]->signal_toggled().connect([this, i]()
                                                      { 
                                                        BinaryDB::update_<int>(i + 10, Config::list_view_ticket[i]->get_active()); 
                                                        Config::TextView::preview_ticket();
                                                       });
    }
    this->list_config_visualizacion->signal_row_activated().connect([this](Gtk::ListBoxRow *row)
                                                                    {
        Config::list_view_ticket[row->get_index()]->get_active() ? Config::list_view_ticket[row->get_index()]->set_active(false) : Config::list_view_ticket[row->get_index()]->set_active();
        BinaryDB::update_<int>(row->get_index() + 10, Config::list_view_ticket[row->get_index()]->get_active()); });

    
    Config::list_config_test->signal_row_activated().connect([](Gtk::ListBoxRow *row){
        std::string command = "echo \"" + Config::TextView::preview_ticket() + "\" | lp";
        std::system(command.c_str());
        });
}

