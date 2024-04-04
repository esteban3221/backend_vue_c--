#pragma once
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