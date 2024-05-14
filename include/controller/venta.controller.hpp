#pragma once
#include <gtkmm.h>
#include <crow/app.h>
#include <crow/middlewares/session.h>
#include <chrono>
#include "SSPComs.h"
#include <mutex>

using Session = crow::SessionMiddleware<crow::InMemoryStore>;
class venta_controller : public Gtk::Frame
{
private:
    unsigned DENOMINATION[7] = {0,20,50,100,200,500,1000};

    SSP_COMMAND_SETUP ssp_setup;
    SSP_POLL_DATA poll;
    SSP_PORT port;

    Glib::Dispatcher m_Dispatcher;

    //referencias vista principal
    crow::App<crow::CookieParser, Session> &app;
    Gtk::Stack &main_stack;
    Gtk::Box *&box_main , box_venta;

    Gtk::ListBox list_venta;
    Gtk::ListBoxRow BXRW1,BXRW2,BXRW3,BXRW4;
    Gtk::Box r1, r2, r3, r4;
    Gtk::Label lbl_monto_total ,lbl_recibido, lbl_cambio , lbl_faltante, lbl_mensaje_fin, lbl_titulo;
    Gtk::Entry ety_monto_total, ety_recibido, ety_cambio, ety_faltante;

    crow::response Test_venta(const crow::request& req);
    void init_ui();
    bool init_perifericos();

    /*
                      TEST
        Total           |            $15.00
        Recibido        |            $10.00
        Falta           |            $5.00
                        |
        Cambio          |            $0.00      

        Gracias por su compra no olvide su ticket.
    */

public:
    venta_controller(crow::App<crow::CookieParser, Session> &app_, Gtk::Stack &main_stack_, Gtk::Box *&box_main_);
    ~venta_controller();
};