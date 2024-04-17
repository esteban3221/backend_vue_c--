#include "nip_view.hpp"

nip_view::nip_view(Gtk::Box *&box_principal_, Gtk::Stack &main_stack_) : box_principal(box_principal_), main_stack(main_stack_)
{
    db.init();
    /*
    id

    1       nip
    2       razon social
    3       direccion
    4       rfc
    5       Contacto
    6       Agradecimiento
    7       status impresion
    8       status inalambrica
    */
    if (db.length() == 0)
    {
        db.adding_string("123456"); // nip de fabrica
        db.adding_string("");
        db.adding_string("");
        db.adding_string("");
        db.adding_string("");
        db.adding_string("");

        db.adding_number<int>(true);
        db.adding_number<int>(true);

        db.save();
    }

    builder->add_from_string(this->XML);

    this->box_nip = builder->get_widget<Gtk::Box>("box_nip");
    this->btn_nip_back = builder->get_widget<Gtk::Button>("btn_nip_back");
    this->ety_pin = builder->get_widget<Gtk::PasswordEntry>("ety_pin");
    this->btn_nip_del = builder->get_widget<Gtk::Button>("btn_nip_del");
    this->btn_nip_enter= builder->get_widget<Gtk::Button>("btn_nip_enter");

    for (size_t i = 0; i < this->botonera_nip.size(); i++)
    {
        this->botonera_nip[i] = builder->get_widget<Gtk::Button>("btn_nip_" + std::to_string(i));
    }

    ety_pin->set_show_peek_icon();

    this->set_child(*box_nip);

    // señales
    this->btn_nip_back->signal_clicked().connect([this]() { ety_pin->set_text(""); main_stack.set_visible_child(*box_principal); });
    for (int i = 0; i < this->botonera_nip.size(); i++) this->botonera_nip[i]->signal_clicked().connect([this, i]() -> void { ety_pin->set_text(ety_pin->get_text() + std::to_string(i)); });
    this->btn_nip_del->signal_clicked().connect([this]() { ety_pin->set_text(""); });
}

inline bool nip_view::is_nip_ok()
{
    return db.get_string(1) == ety_pin->get_text().operator std::string();
}

nip_view::~nip_view()
{
}