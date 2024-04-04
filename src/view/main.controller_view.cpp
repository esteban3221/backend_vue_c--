#include "main.controller_view.hpp"

main_controller::main_controller(/* args */)
{
    this->set_default_size(800, 600);
    this->set_title("Prueba Final");

    this->lbl_main.set_text("text main");
    this->set_child(main_stack);

    main_stack.add(lbl_main);
    main_stack.add(*a);

    main_stack.set_transition_type(Gtk::StackTransitionType::SLIDE_LEFT_RIGHT);

    CROW_ROUTE(this->app, "/rutaPadre") (std::bind(&main_controller::runTest, this));
    CROW_ROUTE(this->app, "/reset") (std::bind(&main_controller::reset, this));

    std::thread([&]() { this->app.port(8080).run(); }).detach();
}

std::string main_controller::runTest()
{
    main_stack.set_visible_child(lbl_main);
    this->lbl_main.set_text("Se ingreso ala ruta padre");
    return "ruta Padre";
}

std::string main_controller::reset()
{
    this->lbl_main.set_text("text main");
    return "text main";
}

main_controller::~main_controller()
{
}