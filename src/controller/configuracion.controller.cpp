#include "configuracion.controller.hpp"

configuracion_controller::configuracion_controller(crow::App<crow::CookieParser, Session> &app_) : app(app_)
{
    dispatcher.connect(sigc::mem_fun(*this, &configuracion_controller::on_dispatcher_emit));

    CROW_ROUTE(app, "/configuracion/actualizaImpresion").methods("POST"_method)(sigc::mem_fun(*this, &configuracion_controller::actualizaImpresion));
    CROW_ROUTE(app, "/configuracion/actualizaInformacion").methods("POST"_method)(sigc::mem_fun(*this, &configuracion_controller::actualizaInformacion));

    CROW_ROUTE(app, "/configuracion/getInformacion").methods("GET"_method)(sigc::mem_fun(*this, &configuracion_controller::getInformacion));
    CROW_ROUTE(app, "/configuracion/getInformacionSistema").methods("GET"_method)(sigc::mem_fun(*this, &configuracion_controller::getInformacionSistema));
    CROW_ROUTE(app, "/configuracion/testImpresion").methods("GET"_method)(sigc::mem_fun(*this, &configuracion_controller::testImpresion));
    CROW_ROUTE(app, "/configuracion/reiniciar").methods("GET"_method)(sigc::mem_fun(*this, &configuracion_controller::reiniciar));
    CROW_ROUTE(app, "/configuracion/apagar").methods("GET"_method)(sigc::mem_fun(*this, &configuracion_controller::apagar));
}

crow::response configuracion_controller::actualizaImpresion(const crow::request &req)
{
    try
    {
        auto &session = app.get_context<Session>(req);

        if (auto status = Helper::User::validPermissions(req, session, Helper::User::allRoles);
            status.first != crow::status::OK)
        {
            return crow::response(status.first);
        }
        else
        {
            //    {
            //     "remoto" : true,
            //     "agradecimiento" : true,
            //     "fecha" : true,
            //     "direccion" : true,
            //     "rfc" : true,
            //     "vendedor" : true,
            //     "contacto" : true
            //     }
            auto bodyParams = crow::json::load(req.body);

            this->dispatch_to_gui([bodyParams]
                                  {
                Config::switch_impresion->set_active(bodyParams["remoto"].b());

                Config::list_view_ticket[0]->set_active(bodyParams["agradecimiento"].b());
                Config::list_view_ticket[1]->set_active(bodyParams["fecha"].b());
                Config::list_view_ticket[2]->set_active(bodyParams["direccion"].b());
                Config::list_view_ticket[3]->set_active(bodyParams["rfc"].b());
                Config::list_view_ticket[4]->set_active(bodyParams["vendedor"].b());
                Config::list_view_ticket[5]->set_active(bodyParams["contacto"].b()); });

                Helper::System::showNotify("Impresion","Se actualizo configuración de impresion.","dialog-information");

            return crow::response(status.first);
        }
    }
    catch (const std::exception &e)
    {
        return crow::response(crow::CONFLICT, e.what());
    }
}

crow::response configuracion_controller::actualizaInformacion(const crow::request &req)
{
    try
    {
        auto &session = app.get_context<Session>(req);

        if (auto status = Helper::User::validPermissions(req, session, Helper::User::allRoles);
            status.first != crow::status::OK)
        {
            return crow::response(status.first);
        }
        else
        {
            auto bodyParams = crow::json::load(req.body);

            const std::string a[5] = {
                bodyParams["razonSocial"].s(),
                bodyParams["direccion"].s(),
                bodyParams["rfc"].s(),
                bodyParams["contacto"].s(),
                bodyParams["agradecimiento"].s()};
            this->dispatch_to_gui([a]
                                  {
                                      for (int i = 0; i < 5; i++)
                                      {
                                          Config::list_ety_datos[i]->set_text(a[i].c_str());
                                      }
                                  });

            Helper::System::showNotify("Información","Se actualizaron datos de la empresa.","dialog-information");

            return crow::response(status.first);
        }
    }
    catch (const std::exception &e)
    {
        return crow::response(crow::CONFLICT, e.what());
    }
}

crow::response configuracion_controller::getInformacion(const crow::request &req)
{
    auto &session = app.get_context<Session>(req);

    if (auto status = Helper::User::validPermissions(req, session,
                                                       {Helper::User::Rol::Configuracion});
        status.first != crow::status::OK)
    {
        return crow::response(status.first);
    }
    else
    {
        crow::json::wvalue response_json;
        response_json["razonSocial"] = Config::list_ety_datos[0]->get_text();
        response_json["direccion"] = Config::list_ety_datos[1]->get_text();
        response_json["rfc"] = Config::list_ety_datos[2]->get_text();
        response_json["contacto"] = Config::list_ety_datos[3]->get_text();
        response_json["agradecimiento"] = Config::list_ety_datos[4]->get_text();
        return crow::response(response_json);
    }
}

crow::response configuracion_controller::testImpresion(const crow::request &req)
{
    auto &session = app.get_context<Session>(req);

    if (auto status = Helper::User::validPermissions(req, session, {Helper::User::Rol::Configuracion});
        status.first != crow::status::OK)
    {
        return crow::response(status.first);
    }
    else
    {
        std::string command = "echo \"" + Config::TextView::preview_ticket() + "\" | lp";
        std::system(command.c_str());

        Helper::System::showNotify("Impresión","Se imprimio hoja de prueba.","dialog-information");
        
        return crow::response(crow::status::OK);
    }
}

crow::response configuracion_controller::reiniciar(const crow::request &req)
{
    auto &session = app.get_context<Session>(req);

    if (auto status = Helper::User::validPermissions(req, session, {Helper::User::Rol::Apagar_Equipo});
        status.first != crow::status::OK)
    {
        return crow::response(status.first);
    }
    else
    {
        Helper::System::exec("shutdown -r +1 &");
        app.stop();
        Helper::System::showNotify("Sistema","Se reiniciara el sistema en un minuto.\n"
        "Servicio de api rest esta desactivado y no recibira mas solicitudes.",
        "dialog-information");
        return crow::response(crow::status::OK);
    }
}

crow::response configuracion_controller::apagar(const crow::request &req)
{
    auto &session = app.get_context<Session>(req);

    if (auto status = Helper::User::validPermissions(req, session, {Helper::User::Rol::Apagar_Equipo});
        status.first != crow::status::OK)
    {
        return crow::response(status.first);
    }
    else
    {
        Helper::System::exec("shutdown +1 &");
        app.stop();
        Helper::System::showNotify("Sistema","Se apagara el sistema en un minuto.\n"
        "Servicio de api rest esta desactivado y no recibira mas solicitudes.",
        "dialog-information");
        return crow::response(crow::status::OK);
    }
}

crow::response configuracion_controller::getInformacionSistema(const crow::request &req)
{
    auto &session = app.get_context<Session>(req);

    if (auto status = Helper::User::validPermissions(req, session, {Helper::User::Rol::Configuracion});
        status.first != crow::status::OK)
    {
        return crow::response(status.first);
    }
    else
    {
        const std::string &parent{"cat /sys/devices/virtual/dmi/id/"};
        const std::string &a{parent + "board_vendor"};
        const std::string &b{parent + "product_name"};

        crow::json::wvalue response_json;

        response_json["hostname"] = Helper::System::exec("cat /etc/hostname");
        response_json["model"] = Helper::System::exec(a.c_str()).empty() ? Helper::System::exec("cat /proc/device-tree/model") : Helper::System::exec(a.c_str()) + " " + Helper::System::exec(b.c_str());
        response_json["processor"] = Helper::System::exec("lscpu | grep -E 'Nombre del modelo|Model name' | awk -F': ' '{print $2}'");
        response_json["ram"] = Helper::System::exec("grep MemTotal /proc/meminfo | awk '{print $2/1024/1024 \" GB\"}' ");
        response_json["memory"] = Helper::System::exec("lsblk -o SIZE -b | head -2 | tail -1 | awk '{print $1/1024/1024/1024 \" GB\"}'");

        return crow::response(response_json);
    }
}

void configuracion_controller::dispatch_to_gui(std::function<void()> func)
{
    {
        std::lock_guard<std::mutex> lock(dispatch_queue_mutex);
        dispatch_queue.push(func);
    }
    dispatcher.emit();
}

void configuracion_controller::on_dispatcher_emit()
{
    std::function<void()> func;
    {
        std::lock_guard<std::mutex> lock(dispatch_queue_mutex);
        if (!dispatch_queue.empty())
        {
            func = dispatch_queue.front();
            dispatch_queue.pop();
        }
    }
    if (func)
    {
        func();
    }
}

configuracion_controller::~configuracion_controller()
{
}
