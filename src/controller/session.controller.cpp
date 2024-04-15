#include "session.controller.hpp"

session_controller::session_controller(crow::App<crow::CookieParser, Session> &app_) : app(app_)
{
    // test
    CROW_ROUTE(app, "/session/verToken").methods("GET"_method)(sigc::mem_fun(*this, &session_controller::verTokens));

    CROW_ROUTE(app, "/session/login").methods("POST"_method)(sigc::mem_fun(*this, &session_controller::login));
    CROW_ROUTE(app, "/session/altaUsuario").methods("POST"_method)(sigc::mem_fun(*this, &session_controller::altaUsuario));
    CROW_ROUTE(app, "/session/bajaUsuario").methods("POST"_method)(sigc::mem_fun(*this, &session_controller::bajaUsuario));
    CROW_ROUTE(app, "/session/logoutUsuario").methods("POST"_method)(sigc::mem_fun(*this, &session_controller::logout));
}

session_controller::~session_controller()
{
}

// solo test
std::string session_controller::verTokens(const crow::request &req)
{
    auto &session = app.get_context<Session>(req);
    auto keys = session.keys();

    std::string out;
    for (const auto &key : keys)
        // .string(key) converts a value of any type to a string
        out += key + " | " + session.string(key) ;
    return out;
}

crow::response session_controller::login(const crow::request &req)
{
    try
    {
        if (req.body.empty())
            return crow::response(crow::status::BAD_REQUEST);

        auto &session = app.get_context<Session>(req);
        auto bodyParams = req.get_body_params();
        std::string grantType{bodyParams.pop("grant_type")};
        std::string username{bodyParams.pop("username")};
        std::string password{bodyParams.pop("password")};

        auto token = jwt::create()
                         .set_type("JWS")
                         .set_issuer("auth0")
                         .sign(jwt::algorithm::hs256{Glib::DateTime::create_now_local().format("%Y%m%d%H%M%S")});

        if (grantType == "password")
        {
            if (M_usuarios.validaUsuario(username, password))
            {
                session.set(username, token);

                crow::json::wvalue response_json;
                response_json["access_token"] = token;
                response_json["expires_in"] = 3600000;
                response_json["token_type"] = "bearer";
                response_json["userName"] = username;
                return crow::response{response_json};
            }
            else
                return crow::response(crow::status::UNAUTHORIZED,"UNAUTHORIZED");
        }
        else if (grantType == "fingerprint")
        {
            if (M_usuarios.validaUsuario(username))
            {
                session.set(username, token);

                crow::json::wvalue response_json;
                response_json["access_token"] = token;
                response_json["expires_in"] = 3600000;
                response_json["token_type"] = "bearer";
                response_json["userName"] = username;
                return crow::response{response_json};
            }
            else
                return crow::response(crow::status::UNAUTHORIZED,"UNAUTHORIZED");
        }
        else
            return crow::response(crow::status::UNAVAILABLE_FOR_LEGAL_REASONS);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return crow::response(crow::status::INTERNAL_SERVER_ERROR);
    }
}

crow::response session_controller::altaUsuario(const crow::request &req)
{
    if (req.body.empty())
        return crow::response(crow::status::EXPECTATION_FAILED);
    auto x = crow::json::load(req.body);
    return crow::response(crow::status::EXPECTATION_FAILED);
}
crow::response session_controller::bajaUsuario(const crow::request &req)
{
    return crow::response(crow::status::EXPECTATION_FAILED);
}
crow::response session_controller::logout(const crow::request &req)
{
    return crow::response(crow::status::EXPECTATION_FAILED);
}