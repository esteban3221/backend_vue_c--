#include "main.controller_view.hpp"

main_controller::main_controller(/* args */)
{

    this->set_default_size(800, 480);
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
    this->img_main_logo = builder->get_widget<Gtk::Image>("img_main_logo");
    this->lbl_main = builder->get_widget<Gtk::Label>("lbl_main");
    this->btn_logo_nip = builder->get_widget<Gtk::Button>("btn_logo_nip");
    this->btn_pill = builder->get_widget<Gtk::Button>("btn_pill");

    nip_set_view = Gtk::manage(new nip_sett_view(box_principal, main_stack, configView));

    // Registro pantallas
    this->main_stack.add(*this->box_principal);
    this->main_stack.add(*this->venta);
    this->main_stack.add(*this->nip);
    this->main_stack.add(*this->configView);
    this->main_stack.add(*this->nip_set_view);

    this->main_stack.set_transition_type(Gtk::StackTransitionType::SLIDE_LEFT_RIGHT);

    
    this->server_running_ = true;
    this->server_thread_ = std::thread([this]() { this->app.port(44333).run(); });


    this->main_stack.set_visible_child(*this->box_principal);
    //this->main_stack.set_visible_child(*this->venta);

    //señales
    this->btn_logo_nip->signal_clicked().connect(sigc::mem_fun (*this, &main_controller::entra_config));

    //extrapolacion de datos desde Config
    this->img_main_logo->property_file() =  BinaryDB::select_<std::string>(16);
    this->lbl_main->set_text(BinaryDB::select_<std::string>(17));

    //const Glib::RefPtr<Gdk::Monitor> &monitor
    // Glib::RefPtr<Gdk::Display> display = Gdk::Monitor::get_display();
    // std::cout << '\n' << display->get_monitors()->get_item_type() << '\n';
    // auto monitor = Glib::RefPtr<Gdk::Monitor>::cast_dynamic(display);
    // this->fullscreen_on_monitor(display);
    
}

void main_controller::entra_config()
{
    if(this->cont_click_logo >= 4)
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
        if (this->server_running_) {
        this->app.stop();
        if (this->server_thread_.joinable()) {
            this->server_thread_.join();
        }
        this->server_running_ = false;
    }
}