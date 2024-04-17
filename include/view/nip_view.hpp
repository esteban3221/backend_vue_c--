#pragma once
#include <gtkmm.h>
#include "binaryfile.hpp"
#include <vector>

class nip_view : public Gtk::Frame
{
private:
    const char *XML =
        "<interface>"
        "<requires lib=\"gtk\" version=\"4.0\"/>"
        "<object class=\"GtkBox\" id=\"box_nip\">"
        "<property name=\"orientation\">1</property>"
        "<property name=\"spacing\">30</property>"
        "<property name=\"homogeneous\">false</property>"
        "<property name=\"margin-bottom\">10</property>"
        "<property name=\"margin-start\">100</property>"
        "<property name=\"margin-end\">100</property>"
        "<property name=\"margin-top\">10</property>"
        "<child>"
        "<object class=\"GtkPasswordEntry\" id=\"ety_pin\">"
        "<property name=\"hexpand\">true</property>"
        "<property name=\"halign\">0</property>"
        "<property name=\"show-peek-icon\">false</property>"
        "<property name=\"placeholder-text\">Digite NIP</property>"
        "<property name=\"xalign\">0.5</property>"
        "<property name=\"height-request\">100</property>"
        "<property name=\"max-width-chars\">8</property>"
        "<style>"
        "<class name=\"title-1\"/>"
        "</style>"
        "</object>"
        "</child>"
        "<child>"
        "<object class=\"GtkGrid\">"
        "<property name=\"vexpand\">true</property>"
        "<property name=\"column-homogeneous\">true</property>"
        "<property name=\"row-homogeneous\">true</property>"
        "<property name=\"column-spacing\">10</property>"
        "<property name=\"row-spacing\">10</property>"
        "<property name=\"halign\">0</property>"
        "<style>"
        "<class name=\"title-1\"/>"
        "</style>"
        "<child>"
        "<object class=\"GtkButton\" id=\"btn_nip_1\">"
        "<property name=\"label\">1</property>"
        "<layout>"
        "<property name=\"column\">0</property>"
        "<property name=\"row\">0</property>"
        "</layout>"
        "</object>"
        "</child>"
        "<child>"
        "<object class=\"GtkButton\" id=\"btn_nip_2\">"
        "<property name=\"label\">2</property>"
        "<layout>"
        "<property name=\"column\">1</property>"
        "<property name=\"row\">0</property>"
        "</layout>"
        "</object>"
        "</child>"
        "<child>"
        "<object class=\"GtkButton\" id=\"btn_nip_3\">"
        "<property name=\"label\">3</property>"
        "<layout>"
        "<property name=\"column\">2</property>"
        "<property name=\"row\">0</property>"
        "</layout>"
        "</object>"
        "</child>"
        "<child>"
        "<object class=\"GtkButton\" id=\"btn_nip_4\">"
        "<property name=\"label\">4</property>"
        "<layout>"
        "<property name=\"column\">0</property>"
        "<property name=\"row\">1</property>"
        "</layout>"
        "</object>"
        "</child>"
        "<child>"
        "<object class=\"GtkButton\" id=\"btn_nip_5\">"
        "<property name=\"label\">5</property>"
        "<layout>"
        "<property name=\"column\">1</property>"
        "<property name=\"row\">1</property>"
        "</layout>"
        "</object>"
        "</child>"
        "<child>"
        "<object class=\"GtkButton\" id=\"btn_nip_6\">"
        "<property name=\"label\">6</property>"
        "<layout>"
        "<property name=\"column\">2</property>"
        "<property name=\"row\">1</property>"
        "</layout>"
        "</object>"
        "</child>"
        "<child>"
        "<object class=\"GtkButton\" id=\"btn_nip_7\">"
        "<property name=\"label\">7</property>"
        "<layout>"
        "<property name=\"column\">0</property>"
        "<property name=\"row\">2</property>"
        "</layout>"
        "</object>"
        "</child>"
        "<child>"
        "<object class=\"GtkButton\" id=\"btn_nip_8\">"
        "<property name=\"label\">8</property>"
        "<layout>"
        "<property name=\"column\">1</property>"
        "<property name=\"row\">2</property>"
        "</layout>"
        "</object>"
        "</child>"
        "<child>"
        "<object class=\"GtkButton\" id=\"btn_nip_9\">"
        "<property name=\"label\">9</property>"
        "<layout>"
        "<property name=\"column\">2</property>"
        "<property name=\"row\">2</property>"
        "</layout>"
        "</object>"
        "</child>"
        "<child>"
        "<object class=\"GtkButton\" id=\"btn_nip_0\">"
        "<property name=\"label\">0</property>"
        "<layout>"
        "<property name=\"column\">1</property>"
        "<property name=\"row\">3</property>"
        "</layout>"
        "</object>"
        "</child>"
        "<child>"
        "<object class=\"GtkButton\" id=\"btn_nip_del\">"
        "<property name=\"icon-name\">edit-clear-symbolic</property>"
        "<layout>"
        "<property name=\"column\">3</property>"
        "<property name=\"row\">3</property>"
        "</layout>"
        "</object>"
        "</child>"
        "<child>"
        "<object class=\"GtkButton\" id=\"btn_nip_enter\">"
        "<property name=\"icon-name\">keyboard-enter-symbolic</property>"
        "<layout>"
        "<property name=\"column\">3</property>"
        "<property name=\"row-span\">3</property>"
        "</layout>"
        "</object>"
        "</child>"
        "<child>"
        "<object class=\"GtkButton\" id=\"btn_nip_back\">"
        "<property name=\"icon-name\">go-previous-symbolic</property>"
        "<layout>"
        "<property name=\"column\">0</property>"
        "<property name=\"row\">3</property>"
        "</layout>"
        "</object>"
        "</child>"
        "</object>"
        "</child>"
        "</object>"
        "</interface>";
    //archivo binario local
    //opt/maxicajero/src/data
    std::binaryfile db = "datab";
    //referencia nodo padre

    Gtk::Box *&box_principal;
    Gtk::Stack &main_stack;

    //Constructor desde XML

    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();

    //referencias de XML

    Gtk::Box *box_nip = nullptr;
    Gtk::Button *btn_nip_back = nullptr;
    Gtk::PasswordEntry *ety_pin = nullptr;
    std::vector<Gtk::Button *> botonera_nip = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
    Gtk::Button *btn_nip_del = nullptr;
    Gtk::Button *btn_nip_enter = nullptr;

    //funciones internas

    inline bool is_nip_ok();

public:
    nip_view(Gtk::Box *&box_principal_, Gtk::Stack &main_stack_);
    ~nip_view();
};
