#include "main.controller_view.hpp"

main_controller::main_controller(/* args */)
{
    this->set_default_size(800, 600);
    this->fullscreen();
    this->set_title("MaxiCajero");

    this->lbl_main.set_text("BIenvenido a Maxicajero");
    this->lbl_main.set_hexpand();
    this->box_principal.set_margin_bottom(10);
    this->box_principal.set_margin_end(10);
    this->box_principal.set_margin_start(10);
    this->box_principal.set_margin_top(10);
    this->box_principal.append(this->lbl_main);
    this->box_principal.set_hexpand();
    this->box_principal.set_vexpand();
    this->set_child(main_stack);

    //Registro pantallas
    main_stack.add(this->box_principal);

    main_stack.set_transition_type(Gtk::StackTransitionType::SLIDE_LEFT_RIGHT);

    std::thread([&]() { this->app.port(8080).run(); }).detach();
}

main_controller::~main_controller()
{
    this->app.stop();
}