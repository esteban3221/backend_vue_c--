#include "session.controller.hpp"

session_controller::session_controller(crow::App<crow::CookieParser, Session> &app_) : app(app_)
{
    dispatcher.connect(sigc::mem_fun(*this, &session_controller::on_dispatcher_emit));
    // test
    CROW_ROUTE(app, "/session/verToken").methods("GET"_method)(sigc::mem_fun(*this, &session_controller::verTokens));

    CROW_ROUTE(app, "/session/login").methods("POST"_method)(sigc::mem_fun(*this, &session_controller::login));
    CROW_ROUTE(app, "/session/logout").methods("POST"_method)(sigc::mem_fun(*this, &session_controller::logout));

    CROW_ROUTE(app, "/session/altaUsuario").methods("POST"_method)(sigc::mem_fun(*this, &session_controller::altaUsuario));
    CROW_ROUTE(app, "/session/bajaUsuario").methods("POST"_method)(sigc::mem_fun(*this, &session_controller::bajaUsuario));
    CROW_ROUTE(app, "/session/modificaUsuario").methods("POST"_method)(sigc::mem_fun(*this, &session_controller::modificaUsuario));
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
        out += key + " | " + session.string(key) + '\n';
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
                         .set_type("Bearer")
                         .set_issuer("auth0")
                         .sign(jwt::algorithm::hs256{Glib::DateTime::create_now_local().format("%Y%m%d%H%M%S-%A")});

        if (grantType == "password")
        {
            if (this->M_usuarios.validaUsuario(username, password))
            {
                session.set(username, token);

                crow::json::wvalue response_json;
                response_json["access_token"] = token;
                response_json["expires_in"] = 15 * 60 * 1'000;
                response_json["token_type"] = "bearer";
                response_json["userName"] = username;
                return crow::response{response_json};
            }
            else
                return crow::response(crow::status::UNAUTHORIZED, "UNAUTHORIZED");
        }
        else if (grantType == "fingerprint")
        {
            if (this->M_usuarios.validaUsuario(username))
            {
                session.set(username, token);

                crow::json::wvalue response_json;
                response_json["access_token"] = token;
                response_json["expires_in"] = 15 * 60 * 1'000;
                response_json["token_type"] = "bearer";
                response_json["userName"] = username;
                return crow::response{response_json};
            }
            else
                return crow::response(crow::status::UNAUTHORIZED, "UNAUTHORIZED");
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
    try
    {
        auto &session = app.get_context<Session>(req);

        if (auto status = Helper::System::validPermissions(req, session, Helper::System::allRoles);
            status.first != crow::status::OK)
        {
            return crow::response(status.first);
        }
        else
        {
            // {
            //     "username" : "prueba",
            //     "password" : "Nuevo password"
            // }
            auto bodyParams = crow::json::load(req.body);
            std::string username{bodyParams["username"].s()};
            std::string password{bodyParams["password"].s()};
            auto id = this->M_usuarios.altaUsuario(username, password);

            this->dispatch_to_gui([this,id,username] {
                auto row = *(model::ModelUsuarios->append());
                row[model::m_Colunms_usuarios.id] = id;
                row[model::m_Colunms_usuarios.nombre] = username;
            });

            return crow::response(status.first);
        }
    }
    catch (const std::exception &e)
    {
        return crow::response(crow::status::CONFLICT, e.what());
    }
}
crow::response session_controller::bajaUsuario(const crow::request &req)
{
    // auto auth_header = req.get_header_value("Authorization");
    // if (auth_header.empty() || auth_header.substr(0, 7) != "Bearer ")
    //     return crow::response(crow::status::BAD_REQUEST, "Authorization header missing or invalid");
    // auto &session = app.get_context<Session>(req);
    // auto keys = session.keys();
    // for (const auto &key : keys)
    // {
    //     if (auth_header.substr(7) == session.string(key))
    //     {
    //         auto bodyParams = crow::json::load(req.body);
    //         std::string username{bodyParams["username"].s()};
    //         this->M_usuarios.bajaUsuario(username);
    //         return crow::response(crow::status::OK);
    //     }
    //     else
    //         break;
    // }
    auto &session = app.get_context<Session>(req);

    if (auto status = Helper::System::validPermissions(req, session, Helper::System::allRoles);
        status.first != crow::status::OK)
    {
        return crow::response(status.first);
    }
    else
    {
        auto bodyParams = crow::json::load(req.body);
        std::string username{bodyParams["username"].s()};
        this->M_usuarios.bajaUsuario(username);
        Glib::ustring glib_username = Glib::ustring::compose("%1", username);

        for (auto &row : model::ModelUsuarios->children())
        {
            if (row[model::m_Colunms_usuarios.nombre].operator Glib::ustring() == glib_username)
            {
                model::ModelUsuarios->erase(row.get_iter());
                break;
            }
                
        }
        return crow::response(status.first);
    }
    return crow::response(crow::status::CONFLICT);
}
crow::response session_controller::modificaUsuario(const crow::request &req)
{
    try
    {
        auto &session = app.get_context<Session>(req);

        if (auto status = Helper::System::validPermissions(req, session, Helper::System::allRoles);
            status.first != crow::status::OK)
        {
            return crow::response(status.first);
        }
        else
        {
            // {
            //     "username" : "prueba",
            //     "password" : "Nuevo password"
            // }
            auto bodyParams = crow::json::load(req.body);
            std::string username{bodyParams["username"].s()};
            std::string password{bodyParams["password"].s()};
            this->M_usuarios.modificaUsuario(username, password);
            return crow::response(status.first);
        }
    }
    catch (const std::exception &e)
    {
        return crow::response(crow::status::CONFLICT, e.what());
    }
}
crow::response session_controller::logout(const crow::request &req)
{

    auto &session = app.get_context<Session>(req);
    std::string username;
    if (auto status = Helper::System::validUser(req, session, username);
        status != crow::status::OK)
    {
        return crow::response(status);
    }
    else
    {
        session.remove(username);
        return crow::response(crow::status::OK);
    }
    return crow::response(crow::status::CONFLICT);
}


void session_controller::dispatch_to_gui(std::function<void()> func) {
    {
        std::lock_guard<std::mutex> lock(dispatch_queue_mutex);
        dispatch_queue.push(func);
    }
    dispatcher.emit();
}

void session_controller::on_dispatcher_emit() {
    std::function<void()> func;
    {
        std::lock_guard<std::mutex> lock(dispatch_queue_mutex);
        if (!dispatch_queue.empty()) {
            func = dispatch_queue.front();
            dispatch_queue.pop();
        }
    }
    if (func) {
        func();
    }
}