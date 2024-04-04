
#include "main.controller_view.hpp"
// #include "main.controller.hpp"

int main(int argc, char *argv[])
{
    // std::unique_ptr<main_controller> endpointserver= std::make_unique<main_controller>(); 
    // std::thread([&](){endpointserver->run();}).detach();
    auto app = Gtk::Application::create("MaxiCajero.www");
    return app->make_window_and_run<main_controller>(argc, argv);
}
