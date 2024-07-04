#pragma once
#include <gtkmm.h>
#include <crow.h>
#include <crow/middlewares/session.h>
#include "config.hpp"
#include "config_text.hpp"
#include "helper.hpp"
#include <sigc++/sigc++.h>

using Session = crow::SessionMiddleware<crow::InMemoryStore>;
class configuracion_controller : public sigc::trackable
{
private:
    crow::App<crow::CookieParser, Session> &app;

    crow::response actualizaImpresion(const crow::request &req);
    crow::response actualizaInformacion(const crow::request &req);
    crow::response getInformacion(const crow::request &req);
    crow::response testImpresion(const crow::request &req);

    crow::response reiniciar(const crow::request &req);
    crow::response apagar(const crow::request &req);

    crow::response getInformacionSistema(const crow::request &req);

    Glib::Dispatcher dispatcher;
    std::queue<std::function<void()>> dispatch_queue;
    std::mutex dispatch_queue_mutex;

    // interno
    void dispatch_to_gui(std::function<void()> func);
    void on_dispatcher_emit();

public:
    configuracion_controller(crow::App<crow::CookieParser, Session> &app_);
    ~configuracion_controller();
};
