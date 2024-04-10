#pragma once
#include "gtkmm/label.h"
#include <crow.h>
#include <gtkmm.h>
#include <thread>
#include <chrono>

class test_controller : public Gtk::Box
{
private:
    //referencia de atributos principales
    Gtk::Stack &main_stack;
    Gtk::Box &contenedor_main;
    crow::SimpleApp &app;

    //Widgets propios de la clase
    Gtk::Label lbl_hijo;

    //handlers
    crow::response testjson(const crow::request &req);
    std::string runTest();

public:
    test_controller(crow::SimpleApp &app_, Gtk::Stack &main_stack_,Gtk::Box &contenedor_main_);
    ~test_controller();
};