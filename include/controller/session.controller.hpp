#pragma once
#include <crow.h>
#include <crow/middlewares/session.h>
#include <sigc++/sigc++.h>
#include <map>
#include "usuarios.hpp"
#include "usuarios_roles.hpp"
#include <glibmm.h>
#include <jwt-cpp/jwt.h>
#include "helper.hpp"


using Session = crow::SessionMiddleware<crow::InMemoryStore>;
class session_controller : public sigc::trackable
{
private:
    //referencia de atributos principales
    crow::App<crow::CookieParser, Session> &app;
    
    model::usuarios M_usuarios;

    crow::response login(const crow::request &req);
    crow::response altaUsuario(const crow::request &req);
    crow::response bajaUsuario(const crow::request &req);
    crow::response modificaUsuario(const crow::request &req);
    crow::response logout(const crow::request &req);

    crow::response actualizaRol(const crow::request &req);


    Glib::Dispatcher dispatcher;
    std::queue<std::function<void()>> dispatch_queue;
    std::mutex dispatch_queue_mutex;

    //interno
    void dispatch_to_gui(std::function<void()> func);
    void on_dispatcher_emit();

    //test
    std::string verTokens (const crow::request& req);

public:
    session_controller(crow::App<crow::CookieParser, Session> &app_);
    ~session_controller();
};

