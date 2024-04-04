#include "gtkmm/label.h"
#include <crow.h>
#include <gtkmm.h>
#include <thread>
#include <chrono>

class test_controller : public Gtk::Box
{
private:
    Gtk::Stack &main_stack_;
    Gtk::Label lbl_hijo;
    Gtk::Label &lbl_main_;
    crow::SimpleApp &app;

public:
    test_controller(crow::SimpleApp &App, Gtk::Stack &main_stack,Gtk::Label &lbl_main);
    ~test_controller();
};

test_controller::test_controller(crow::SimpleApp &App, Gtk::Stack &main_stack, Gtk::Label &lbl_main) : app(App) , main_stack_(main_stack), lbl_main_(lbl_main)
{
    this->append(lbl_hijo);
    CROW_ROUTE(app, "/rutaTest")
    ([this]()
     {
            this->main_stack_.set_visible_child(*this);
            this->lbl_hijo.set_text("ruta hija");
            std::this_thread::sleep_for(std::chrono::seconds(5));
            this->main_stack_.set_visible_child(lbl_main_);
            return "ruta hija"; });
}

test_controller::~test_controller()
{
}