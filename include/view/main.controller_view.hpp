#pragma once
#include <crow/app.h>
#include "gtkmm/stack.h"
#include "test.controller.hpp"
#include <memory>
#include <gtkmm.h>

class main_controller : public Gtk::Window
{
private:
    // Widgets del front principal
    Gtk::Stack main_stack;
    Gtk::Label lbl_main;

    // controladores hijos
    crow::SimpleApp app;
    std::unique_ptr<test_controller> a = std::make_unique<test_controller>(app,main_stack,lbl_main);

    // funciones padre
    std::string runTest();
    std::string reset();

public:
    main_controller(/* args */);
    ~main_controller();
};