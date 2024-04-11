#pragma once
#include <crow/app.h>
#include "sqlite.hpp"
#include "test.controller.hpp"
#include "session.controller.hpp"
#include "venta.controller.hpp"
#include <memory>
#include <gtkmm.h>

class main_controller : public Gtk::Window
{
private:
    // Widgets del front principal
    Gtk::Stack main_stack;
    Gtk::Box box_principal;
    Gtk::Label lbl_main;

    // controladores test
    //crow::SimpleApp app_;

    crow::App<crow::CookieParser, Session> app{Session{crow::CookieParser::Cookie("session").
    max_age(/*15 minutos vida maxima de tokena*/ 15*60).
    path("/"), 64, crow::InMemoryStore{}}};

    // std::unique_ptr<test_controller> a = std::make_unique<test_controller>(app_,main_stack,box_principal);

    std::unique_ptr<session_controller> session = std::make_unique<session_controller>(app);
    std::unique_ptr<venta_controller> venta = std::make_unique<venta_controller>();

    // funciones padre
    std::string runTest();
    std::string reset();

public:
    main_controller(/* args */);
    ~main_controller();
};