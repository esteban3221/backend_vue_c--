#include "session.controller.hpp"

session_controller::session_controller(crow::App<crow::CookieParser, Session> &app_) : app(app_)
{
    // test
    // CROW_ROUTE(app, "/session/verToken").methods("GET"_method)(sigc::mem_fun(*this, &session_controller::verTokens));

    CROW_ROUTE(app, "/session/login").methods("POST"_method)(sigc::mem_fun(*this, &session_controller::login));
    CROW_ROUTE(app, "/session/altaUsuario").methods("POST"_method)(sigc::mem_fun(*this, &session_controller::altaUsuario));
    CROW_ROUTE(app, "/session/bajaUsuario").methods("POST"_method)(sigc::mem_fun(*this, &session_controller::bajaUsuario));
    CROW_ROUTE(app, "/session/logoutUsuario").methods("POST"_method)(sigc::mem_fun(*this, &session_controller::logout));
}

session_controller::~session_controller()
{
}

std::string session_controller::verTokens(const crow::request &req)
{
    auto &session = app.get_context<Session>(req);
    auto keys = session.keys();

    std::string out;
    for (const auto &key : keys)
        // .string(key) converts a value of any type to a string
        out += "<p> " + key + " = " + session.string(key) + "</p>";
    return out;
}

crow::response session_controller::login(const crow::request &req)
{
    try
    {
        if (req.body.empty())
            return crow::response(crow::status::EXPECTATION_FAILED); // same as crow::response(400)

        // grant_type=password
        if (auto x = req.get_body_params(); x.get("grant_type") == "password")
        {
            auto &session = app.get_context<Session>(req);

            auto key = x.get("username");
            auto value = x.pop("password");

            session.set(key, value);

            auto keys = session.keys();

            crow::json::wvalue response_json;

            response_json["access_token"] = "Prueba de tokens xD";
            response_json["expires_in"] = 3600000;
            response_json["token_type"] = "bearer";
            response_json["userName"] = x.pop("username");

            // std::cout << x << " | " << y << " | " << z <<std::endl;
            return crow::response{response_json};
        }
        return crow::response(crow::status::UNAVAILABLE_FOR_LEGAL_REASONS);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return crow::response(crow::status::RANGE_NOT_SATISFIABLE);
    }
}

crow::response session_controller::altaUsuario(const crow::request &req) 
{
    if (req.body.empty())
            return crow::response(crow::status::EXPECTATION_FAILED);
    auto x = crow::json::load(req.body);
    
}
crow::response session_controller::bajaUsuario(const crow::request &req) 
{

}
crow::response session_controller::logout(const crow::request &req) 
{

}