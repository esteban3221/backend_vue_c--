#pragma once
#include <gtkmm.h>
#include <vector>
#include "wrapbinary.hpp"
#include <filesystem>

#include "view_two.hpp"
#include "view_three.hpp"
#include "view_four.hpp"
#include "view_five.hpp"
#include "view_six.hpp"
#include "view_seven.hpp"

class config_view : public Gtk::Box
{
private:
    /* data */
    const char *XML =
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<interface>"
            "<object class=\"GtkBox\" id=\"left_box\">"
            "<property name=\"spacing\">5</property>"
            "<property name=\"orientation\">1</property>"
            "<child>"
            "<object class=\"GtkButton\" id=\"btn_back_config\">"
                "<property name=\"halign\">0</property>"
                "<property name=\"margin-start\">10</property>"
                "<property name=\"margin-end\">10</property>"
                "<property name=\"has-frame\">false</property>"
                "<property name=\"child\">"
                "<object class=\"GtkBox\">"
                    "<property name=\"spacing\">5</property>"
                    "<child>"
                    "<object class=\"GtkImage\">"
                        "<property name=\"icon-name\">go-previous-symbolic</property>"
                    "</object>"
                    "</child>"
                    "<child>"
                    "<object class=\"GtkLabel\">"
                        "<property name=\"label\">Regresar</property>"
                    "</object>"
                    "</child>"
                "</object>"
                "</property>"
            "</object>"
            "</child>"
            "<child>"
            "<object class=\"GtkSeparator\"></object>"
            "</child>"
            "<child>"
            "<object class=\"GtkStackSidebar\" id=\"stackside\">"
                "<property name=\"vexpand\">true</property>"
            "</object>"
            "</child>"
        "</object>"
        "</interface>";

    //refrencias para volver al vista principal
    Gtk::Box *&box_principal;
    Gtk::Stack &main_stack;
    Gtk::Box *&view_one;

    Gtk::Button *btn_back_config = nullptr;
    Gtk::Box *left_box = nullptr;
    Gtk::StackSidebar *stackside = nullptr;
    Gtk::Stack stacconfig;

    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();

    //funciones internas
    //
    //void test_creat_pdf(Gtk::ListBoxRow *row);}
    std::unique_ptr<view_three> v2 = std::make_unique<view_three>();
    std::unique_ptr<view_two> v3 = std::make_unique<view_two>();
    std::unique_ptr<view_four> v4 = std::make_unique<view_four>();
    std::unique_ptr<view_five> v5 = std::make_unique<view_five>();
    std::unique_ptr<view_six> v6 = std::make_unique<view_six>();
    std::unique_ptr<view_seven> v7 = std::make_unique<view_seven>();

public:
    config_view(Gtk::Box *&box_principal_,Gtk::Stack &main_stack_,Gtk::Box *&view_one_);
    ~config_view();
};