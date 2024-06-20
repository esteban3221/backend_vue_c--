#pragma once
#include <crow/app.h>
#include "sqlite.hpp"

//controllers
#include "test.controller.hpp"
#include "session.controller.hpp"
#include "venta.controller.hpp"

//views
#include "nip_view.hpp"
#include "nip_sett_view.hpp"
#include "config_view.hpp"
//config
#include "view_one.hpp"

#include <memory>
#include <gtkmm.h>

class main_controller : public Gtk::Window
{
private:
    // Widgets del front principal
    const char *XML = "<interface>"
                      "<requires lib=\"gtk\" version=\"4.0\"/>"
                      "<object class=\"GtkBox\" id=\"box_principal\">"
                      "<property name=\"orientation\">1</property>"
                      "<property name=\"homogeneous\">false</property>"
                      "<property name=\"margin-bottom\">10</property>"
                      "<property name=\"margin-start\">10</property>"
                      "<property name=\"margin-end\">10</property>"
                      "<property name=\"margin-top\">10</property>"
                      "<child>"
                      "<object class=\"GtkBox\">"
                      "<property name=\"homogeneous\">true</property>"
                      "<property name=\"orientation\">1</property>"
                      "<child>"
                      "<object class=\"GtkLabel\" id=\"lbl_main\">"
                      "<property name=\"label\">Bienvenido</property>"
                      "<style>"
                      "<class name=\"title-1\"/>"
                      "</style>"
                      "</object>"
                      "</child>"
                      "<child>"
                      "<object class=\"GtkOverlay\">"
                      "<property name=\"child\">"
                      "<object class=\"GtkImage\" id=\"img_main_logo\">"
                      "<property name=\"file\">/home/esteban/Descargas/ll/TicketImages/logo_blanco.png</property>"
                      "<property name=\"pixel-size\">150</property>"
                      "<property name=\"use-fallback\">true</property>"
                      "</object>"
                      "</property>"
                      "<child type=\"overlay\">"
                      "<object class=\"GtkButton\" id=\"btn_logo_nip\">"
                      "<property name=\"label\">Maxicajero</property>"
                      "<property name=\"halign\">3</property>"
                      "<property name=\"valign\">3</property>"
                      "<property name=\"margin-bottom\">150</property>"
                      "<property name=\"margin-start\">150</property>"
                      "<property name=\"margin-end\">150</property>"
                      "<property name=\"margin-top\">150</property>"
                      "<property name=\"opacity\">0</property>"
                      "</object>"
                      "</child>"
                      "</object>"
                      "</child>"
                      "</object>"
                      "</child>"
                      "<child>"
                      "<object class=\"GtkButton\" id=\"btn_pill\">"
                      "<property name=\"label\" translatable=\"true\">1</property>"
                      "<property name=\"halign\">3</property>"
                      "<property name=\"valign\">3</property>"
                      "<property name=\"vexpand\">true</property>"
                      "<property name=\"opacity\">0</property>"
                      "<style>"
                      "<class name=\"pill\"/>"
                      "<class name=\"suggested-action\"/>"
                      "</style>"
                      "</object>"
                      "</child>"
                      "<child>"
                      "<object class=\"GtkLabel\" id=\"lbl_version\">"
                      "<property name=\"label\">Version ; 1.0.0.1</property>"
                      "<property name=\"halign\">2</property>"
                      "<property name=\"valign\">2</property>"
                      "</object>"
                      "</child>"
                      "</object>"
                      "</interface>";

    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();

    Gtk::Stack main_stack;
    Gtk::Box *box_principal = nullptr;
    Gtk::Label *lbl_version = nullptr, *lbl_main = nullptr;
    Gtk::Button *btn_logo_nip = nullptr, *btn_pill = nullptr;
    Gtk::Image *img_main_logo = nullptr;


    //interno
    int cont_click_logo = 0;
    std::thread server_thread_;
    std::atomic<bool> server_running_{false};


    crow::App<crow::CookieParser, Session> app{Session{crow::CookieParser::Cookie("session").max_age(/*15 minutos vida maxima de tokens*/ 15 * 60).path("/"), 64, crow::InMemoryStore{}}};

    //Controllers
    // std::unique_ptr<test_controller> a = std::make_unique<test_controller>(app_,main_stack,box_principal);
    std::unique_ptr<session_controller> session = std::make_unique<session_controller>(app);
    std::unique_ptr<venta_controller> venta = std::make_unique<venta_controller>(app, main_stack , box_principal);

    //Views
    Gtk::Frame *nip_set_view = nullptr;
    Gtk::Box *v1 = Gtk::manage(new view_one(main_stack, *this, lbl_main, img_main_logo, nip_set_view));
    Gtk::Box *configView = Gtk::manage(new config_view(box_principal, main_stack, v1));
    Gtk::Frame *nip = Gtk::manage(new nip_view(box_principal, main_stack, configView));
    

    // funciones padre
    std::string runTest();
    std::string reset();

    void entra_config();

public:
    main_controller(/* args */);
    ~main_controller();

protected:
    std::unique_ptr<Gtk::MessageDialog> m_pDialog;
};