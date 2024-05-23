#include "nip_view.hpp"

nip_view::nip_view(Gtk::Box *&box_principal_, Gtk::Stack &main_stack_,Gtk::Box *&view_config_): 
                                                                               box_principal(box_principal_),
                                                                               main_stack(main_stack_),
                                                                               frame_config(view_config_)
{
    builder->add_from_string(this->XML);

    this->box_nip = builder->get_widget<Gtk::Box>("box_nip");
    this->btn_nip_back = builder->get_widget<Gtk::Button>("btn_nip_back");
    this->ety_pin = builder->get_widget<Gtk::PasswordEntry>("ety_pin");
    this->btn_nip_del = builder->get_widget<Gtk::Button>("btn_nip_del");
    this->btn_nip_enter = builder->get_widget<Gtk::Button>("btn_nip_enter");

    for (size_t i = 0; i < this->botonera_nip.size(); i++)
    this->botonera_nip[i] = builder->get_widget<Gtk::Button>("btn_nip_" + std::to_string(i));

    this->set_child(*box_nip);

    // señales

    for (int i = 0; i < this->botonera_nip.size(); i++) this->botonera_nip[i]->signal_clicked().connect([this, i](){ ety_pin->set_text(ety_pin->get_text() + std::to_string(i)); });
    this->btn_nip_del->signal_clicked().connect([this]() { ety_pin->set_text(""); });
    this->btn_nip_back->signal_clicked().connect(sigc::mem_fun(*this, &nip_view::on_btn_nip_back));
    this->btn_nip_enter->signal_clicked().connect(sigc::mem_fun(*this, &nip_view::on_btn_nip_enter));
}

inline bool nip_view::is_nip_ok()
{
    auto key {BinaryDB::select_<std::string>(1)};
    return key == ety_pin->get_text().operator std::string();
}

void nip_view::on_btn_nip_back()
{
    this->ety_pin->set_text(""); 
    this->main_stack.set_visible_child(*box_principal); 
    this->ety_pin->set_css_classes({"entry","title-1"}); 
    this->ety_pin->property_placeholder_text() = "Ingrese NIP";

}

void nip_view::on_btn_nip_enter()
{
    if (is_nip_ok())
    {
        this->main_stack.set_visible_child(*frame_config);
        this->ety_pin->property_placeholder_text() = "Ingrese NIP";
        this->ety_pin->set_css_classes({"entry","title-1"});
    }
        
    else
    {
        this->ety_pin->property_placeholder_text() = "Error";
        this->ety_pin->set_css_classes({"error","title-1"});
    }
    this->ety_pin->set_text("");
}

nip_view::~nip_view()
{
    
}