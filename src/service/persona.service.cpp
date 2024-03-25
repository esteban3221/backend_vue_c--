#include "persona.service.hpp"

persona_service::persona_service(/* args */)
{
}

persona_service::~persona_service()
{
}

void persona_service::obtener_lista_persona(struct mg_connection *c)
{
    mg_http_reply(c, 200, "Access-Control-Allow-Origin: *\r\nContent-Type: application/json\r\n", getPersonaList().dump().c_str());
}

void persona_service::add_perosna(struct mg_connection *c, struct mg_http_message *hm)
{
    //natvo en c
    //printf("JSON received: %.*s\n", (int)hm->body.len, hm->body.ptr);

    std::string body_json (hm->body.ptr,hm->body.len);

    std::cout << "C++ : " << body_json << std::endl;

    // mg_http_reply(c, 200, "Access-Control-Allow-Origin: *\r\nContent-Type: application/json\r\n", "URI: %.*s", (int) hm->uri.len, hm->uri.ptr);
    mg_http_reply(c, 200, "Access-Control-Allow-Origin: *\r\nContent-Type: application/json\r\n", "{\"status\": 1}\n");

}