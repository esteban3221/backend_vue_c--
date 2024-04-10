#pragma once
#include <crow.h>
#include <sigc++/sigc++.h>
#include <map>
#include <crow/middlewares/session.h>


using Session = crow::SessionMiddleware<crow::InMemoryStore>;
class session_controller : public sigc::trackable
{
private:
    //referencia de atributos principales
    // crow::SimpleApp &app;
    crow::App<crow::CookieParser, Session> &app;

    crow::response login(const crow::request &req);
    crow::response altaUsuario(const crow::request &req);
    crow::response bajaUsuario(const crow::request &req);
    crow::response logout(const crow::request &req);

    //test
    std::string verTokens (const crow::request& req);

public:
    session_controller(crow::App<crow::CookieParser, Session> &app_);
    ~session_controller();
};

