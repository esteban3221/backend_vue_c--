#include "persona.service.hpp"

persona_service::persona_service(/* args */)
{
}

persona_service::~persona_service()
{
}

void persona_service::obtener_lista_persona(struct mg_connection *c)
{
    mg_http_reply(c, 200, "Access-Control-Allow-Origin: *\r\nContent-Type: application/json\r\n", persona::getPersonaList().dump().c_str());
}

void persona_service::add_perosna(struct mg_connection *c, struct mg_http_message *hm)
{
    try
    {
        // natvo en c
        printf("JSON received: %.*s\n", (int)hm->body.len, hm->body.ptr);
        std::string body_json(hm->body.ptr, hm->body.len);
        auto json = nlohmann::json::parse(body_json);
        persona::insertaPersona(json);

        // mg_http_reply(c, 200, "Access-Control-Allow-Origin: *\r\nContent-Type: application/json\r\n", "URI: %.*s", (int) hm->uri.len, hm->uri.ptr);
        mg_http_reply(c, 200, "Access-Control-Allow-Origin: *\r\nContent-Type: application/json\r\n", "{\"status\": 1}\n");
    }
    catch (const std::exception &e)
    {
        std::string error_message = "Error: ";
        error_message += e.what();
        mg_http_reply(c, 500, "Access-Control-Allow-Origin: *\r\nContent-Type: application/json\r\n", ("{\"Error\" : \""+ error_message+" \" }\n").c_str());
    }
}