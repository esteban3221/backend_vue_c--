#include "nip_view.hpp"

nip_view::nip_view(Gtk::Box *&box_principal_, Gtk::Stack &main_stack_, Gtk::Window &main_window_, Gtk::Label *&lbl_main_, Gtk::Image *&img_main_logo_): 
                                                                               box_principal(box_principal_),
                                                                               main_stack(main_stack_),
                                                                               main_window(main_window_),
                                                                               lbl_main(lbl_main_),
                                                                               img_main_logo(img_main_logo_)
{
    main_stack.add(*configView);
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
    this->btn_nip_back->signal_clicked().connect([this]() { ety_pin->set_text(""); main_stack.set_visible_child(*box_principal); ety_pin->set_css_classes({"entry","title-1"}); ety_pin->property_placeholder_text() = "Ingrese NIP";});
    for (int i = 0; i < this->botonera_nip.size(); i++) this->botonera_nip[i]->signal_clicked().connect([this, i](){ ety_pin->set_text(ety_pin->get_text() + std::to_string(i)); });
    this->btn_nip_del->signal_clicked().connect([this]() { ety_pin->set_text(""); });
    this->btn_nip_enter->signal_clicked().connect(sigc::mem_fun(*this, &nip_view::on_btn_nip_enter));
}

inline bool nip_view::is_nip_ok()
{
    return configView->get_nip() == ety_pin->get_text().operator std::string();
}

void nip_view::on_btn_nip_enter()
{
    if (is_nip_ok())
    {
        main_stack.set_visible_child(*configView);
        ety_pin->property_placeholder_text() = "Ingrese NIP";
        ety_pin->set_css_classes({"entry","title-1"});
    }
        
    else
    {
        ety_pin->property_placeholder_text() = "Error";
        ety_pin->set_css_classes({"error","title-1"});
    }
    ety_pin->set_text("");
}

const std::string nip_view::set_image_path(){
    return configView->set_image_path();
}
const std::string nip_view::set_label_main_text(){
    return configView->set_label_main_text();
}

nip_view::~nip_view()
{
    
}