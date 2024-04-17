#include "main.controller_view.hpp"

main_controller::main_controller(/* args */)
{
    this->set_default_size(800, 600);
    this->set_title("MaxiCajero");
    this->set_child(main_stack);

    try
    {
        builder->add_from_string(this->XML);
    }
    catch (const Glib::Error &e)
    {
        m_pDialog.reset(new Gtk::MessageDialog(*this, "Ui Error",
                                               false /* use_markup */, Gtk::MessageType::ERROR,
                                               Gtk::ButtonsType::OK, true /* modal */));
        m_pDialog->set_secondary_text(e.what());
        m_pDialog->set_hide_on_close(true);
        m_pDialog->signal_response().connect([&](int response_id) { this->m_pDialog->close(); });

        m_pDialog->show();
    }

    // obtencion widget desde XML
    this->box_principal = builder->get_widget<Gtk::Box>("box_principal");
    this->lbl_version = builder->get_widget<Gtk::Label>("lbl_version");
    this->btn_logo_nip = builder->get_widget<Gtk::Button>("btn_logo_nip");
    this->btn_pill = builder->get_widget<Gtk::Button>("btn_pill");

    // Registro pantallas
    this->main_stack.add(*this->box_principal);
    this->main_stack.add(*this->venta);
    this->main_stack.add(*this->nip);

    this->main_stack.set_transition_type(Gtk::StackTransitionType::SLIDE_LEFT_RIGHT);

    std::thread([&]() { this->app.port(44333).run(); }).detach();

    //señales
    this->btn_logo_nip->signal_clicked().connect(sigc::mem_fun (*this, &main_controller::entra_config));
}

void main_controller::entra_config()
{
    if(this->cont_click_logo > 3)
    {
        this->main_stack.set_visible_child(*nip);
        this->cont_click_logo = 0;
        this->btn_pill->set_opacity(0);
    }
    else{
        this->cont_click_logo++;
        this->btn_pill->set_opacity(1);
        this->btn_pill->set_label(std::to_string(this->cont_click_logo));
    }
}

main_controller::~main_controller()
{
    this->app.stop();
}