#pragma once
#include <gtkmm.h>
#include <crow/app.h>
#include <crow/middlewares/session.h>
#include <chrono>
#include <mutex>
#include <atomic>

#include "helper.hpp"
#include "acciones.hpp"

using Session = crow::SessionMiddleware<crow::InMemoryStore>;
class venta_controller : public Gtk::Frame
{
private:
    unsigned DENOMINATION[7] = {0,20,50,100,200,500,1000};

    //Glib::Dispatcher m_Dispatcher;
    sigc::connection M_timeout_venta;
    sigc::connection M_idle_venta;
    sigc::connection M_idle_validator;

    //referencias vista principal
    crow::App<crow::CookieParser, Session> &app;
    Gtk::Stack &main_stack;
    Gtk::Box *&box_main , box_venta;

    Gtk::ListBox list_venta;

    Gtk::ListBoxRow BXRW1,BXRW2,BXRW3,BXRW4;
    Gtk::Box r1, r2, r3, r4, box_action_timeout;
    Gtk::Label lbl_monto_total ,lbl_recibido, lbl_cambio , lbl_faltante, lbl_mensaje_fin, lbl_titulo, lbl_timeout;
    Gtk::Entry ety_monto_total, ety_recibido, ety_cambio, ety_faltante;
    Gtk::Button btn_timeout_cancel , btn_timeout_retry;

    Glib::Dispatcher dispatcher;
    std::queue<std::function<void()>> dispatch_queue;
    std::mutex dispatch_queue_mutex;
    std::atomic<bool> timeout;
    std::atomic<bool> payment_completed;
    int box_state;
    

    crow::response Test_venta(const crow::request& req);

    void reset_gui();

    void update_total(int total);

    void initialize_timeout(int total);

    bool start_payment(Helper::Validator &validator);

    bool process_payment(Helper::Validator &validator, int total);

    void finalize_payment(Helper::Validator &validator, int total);

    void handle_payment_error(Acciones& accion, const std::pair<int, std::string>& status, const std::string& total, const std::string& sum);

    void handle_timeout(Acciones& accion, const std::pair<int, std::string>& status, const std::string& total, const std::string& sum);

    crow::response venta(const crow::request& req);
    crow::response refill(const crow::request& req);
    crow::response refound(const crow::request& req);


    void dispatch_to_gui(std::function<void()> func);
    void init_ui();
    // bool init_perifericos();
    void on_dispatcher_emit();

    

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